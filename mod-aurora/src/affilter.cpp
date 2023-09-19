/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common libraries

  affilter.h

  Angelo Farina - Simone Campanini

***********************************************************************/
#include <aurora.h>

void Aurora::Filter::IIR(Aurora::AudioTrack& dst, Aurora::AudioTrack& src,
		           const double alpha, const double beta, const double gamma,
	               const double mu,    const double sigma)
{
   // code by Angelo Farina
	Aurora::Sample* xt = src.GetData(m_nCh);
	Aurora::Sample* yt = dst.GetData(m_nCh);

	if(!xt || ! yt)
	{
		m_nErr = ERR_DATA;
		return;
	}

	Aurora::SampleCount length = src.GetLength();

	if(length > dst.GetLength())
	{
		m_nErr = ERR_LENGTH_MISMATCH;
		return;
	}

   // x_(n) = 2 * (Alfa * (x_n + Mu * x_nm1 + dbSigma * x_nm2) + dbGamma * y_nm1 - dbBeta * y_nm2) - Basic
   yt[0] = Aurora::Sample( 2.0 * alpha * xt[0] );
   yt[1] = Aurora::Sample( 2.0 * (alpha * (          xt[1]
                                           +    mu * xt[0])
                                           + gamma * yt[0]) );

   for(Aurora::SampleCount n = 2; n < length; n++)
   {
	  // WARNING: feedback components should have minus sign: -> -dbGamma*y_nm1 ???
	  yt[n] = Aurora::Sample( 2.0 * (alpha * (          xt[n]
                                              +    mu * xt[n-1]
                                              + sigma * xt[n-2])
                                              + gamma * yt[n-1]
                                              -  beta * yt[n-2]) );
   }
}

void Aurora::Filter::DoFilter(Aurora::AudioTrack& dst, Aurora::AudioTrack& src,
		              const int type, const double f0, const double fQ)
{
	// code by Angelo Farina
	// Two poles IIR filter
	double alpha = 0.0;
	double mu    = 0.0;
	double sigma = 0.0;

	double theta0 = 2.0 * M_PI * f0 / src.GetRate();
	double d      = 2.0 * std::tan(theta0 / 2.0 / fQ) /
			              std::sin(theta0);
	double beta   = 0.5 * (1.0 - 0.5 * d * std::sin(theta0)) /
			              (1.0 + 0.5 * d * std::sin(theta0));
	double gamma  = (0.5 + beta) * std::cos(theta0);

	switch(type)
	{
		case BPF:
			mu    = 0.0;
			alpha = (0.5 - beta) / 2.0;
			sigma = -1.0;
			break;
		case LPF:
			mu    = 2.0;
			alpha = (0.5 + beta - gamma) / 4.0;
			sigma = 1.0;
			break;
		case HPF:
			mu    = -2.0;
			alpha = (0.5 + beta + gamma) / 4.0;
			sigma = 1.0;
			break;
		case NotchF:
			mu    = -2.0 * cos(theta0);
			alpha = (0.5 + beta) / 2.0;
			sigma = 1.0;
			break;
	}
	IIR(dst, src, alpha, beta, gamma, mu, sigma);
}

void Aurora::Filter::AFilter(Aurora::AudioTrack& dst, Aurora::AudioTrack& src) //... bool preserve = true)
{
	// code by Angelo Farina
	// IIR two-poles filter
	Aurora::Sample* xt = src.GetData(m_nCh);
	Aurora::Sample* yt = dst.GetData(m_nCh);

	if(!xt || ! yt)
	{
		m_nErr = ERR_DATA;
		return;
	}

	Aurora::SampleCount length = src.GetLength();

	if(length > dst.GetLength())
	{
		m_nErr = ERR_LENGTH_MISMATCH;
		return;
	}

	double rate = src.GetRate();

	if(rate != dst.GetRate())
	{
		m_nErr = ERR_RATE_MISMATCH;
		return;
	}

	//  filter 1 - high pass 20.6 Hz
	double c_1  = 1.0 - exp(-2.0 * M_PI * (20.6) / rate);
	double c_1m = 1.0 - c_1;
	//  filter 2 - high pass 20.6 Hz
	double c_2  = 1.0 - exp(-2.0 * M_PI * (20.6) / rate);
	double c_2m = 1.0 - c_2;
	//  filter 3 - high pass 107.7 Hz
	double c_3  = 1.0 - exp(-2.0 * M_PI * (107.7) / rate);
	double c_3m = 1.0 - c_3;
	//  filter 4 - high pass 737.9 Hz
	double c_4  = 1.0 - exp(-2.0 * M_PI * (737.9) / rate);
	double c_4m = 1.0 - c_4;
	//  filter 5 - low pass 12200 Hz
	double c_5  = 1.0 - exp(-2.0 * M_PI * (12200.0) / rate);
	double c_5m = 1.0 - c_5;
	//  filter 6 - low pass 12200 Hz
	double c_6  = 1.0 - exp(-2.0 * M_PI * (12200.0) / rate);
	double c_6m = 1.0 - c_6;

	// the following are delays
	double delta_1 = 0.0;
	double delta_2 = 0.0;
	double delta_3 = 0.0;
	double delta_4 = 0.0;

	double A,B,C,D,E,F;

	Aurora::SampleCount k = 0;
	// The first sample is elaborated separately.
	//   first 4 high pass filters
	A       = xt[k] - delta_1;
	delta_1 = xt[k] * c_1 + delta_1 * c_1m;
	B       = A - delta_2;
	delta_2 = A * c_2 + delta_2 * c_2m;
	C       = B - delta_3;
	delta_3 = B * c_3 + delta_3 * c_3m;
	D       = C - delta_4;
	delta_4 = C * c_4 + delta_4 * c_4m;

	E = D;
	F = D;
	//   2 last low pass filters
	E     = D * c_5 + E * c_5m;
	F     = E * c_6 + F * c_6m;
	yt[k] = Aurora::Sample(F/0.858);

	for(k = 1; k < length; k++)
	{
		//    first 4 high pass filters
		A       = xt[k] - delta_1;
		delta_1 = xt[k] * c_1 + delta_1 * c_1m;
		B       = A - delta_2;
		delta_2 = A * c_2 + delta_2 * c_2m;
		C       = B - delta_3;
		delta_3 = B * c_3 + delta_3 * c_3m;
		D       = C - delta_4;
		delta_4 = C * c_4 + delta_4 * c_4m;

		//    2 last low pass filters
		E     = D * c_5 + E * c_5m;
		F     = E * c_6 + F * c_6m;
		yt[k] = Aurora::Sample(F / 0.858); // correction to make 1kHz gain  0 dB
	}
}

void Aurora::Filter::LFilter(Aurora::AudioTrack& dst, Aurora::AudioTrack& src)
{
	   // code by Angelo Farina

	   double f0,fq;

	   //  filter 1 - high pass 10 Hz
	   f0 = 10.0f; // Hz
	   fq = 0.707f;
	   DoFilter(dst, src, HPF, f0, fq);
	   //  filter 2 - high pass 14 Hz
	   f0 = 14.0f; // Hz
	   fq = 0.707f;
	   DoFilter(dst, src, HPF, f0, fq);
	   //  filter 3 - high pass  20 Hz
	   f0 = 20.0f; // Hz
	   fq = 0.707f;
	   DoFilter(dst, src, HPF, f0, fq);
	   /*
	   //  filter 4 - low pass 20 kHz
	   dbF0 = 20000.0f; // Hz
	   dbFq = 0.707f;
	   Filter(dst, src, LPF, dbF0, dbFq);
	   //  filter 5 - low pass 20.4 kHz
	   dbF0 = 20400.0f; // Hz
	   dbFq = 0.707f;
	   Filter(dst, src, LPF, dbF0, dbFq);
	   //  filter 6 - low pass 20.9 kHz
	   dbF0 = 20900.0f; // Hz
	   dbFq = 0.707f;
	   Filter(dst, src, LPF, dbF0, dbFq);
	   */

}

void Aurora::Filter::ISO_8041_Filter(Aurora::AudioTrack& dst, Aurora::AudioTrack& src)
{
	// Ponderazione Accelerazione Asse Generico secondo norma ISO-8041

	Aurora::Sample* xt = src.GetData(m_nCh);
	Aurora::Sample* yt = dst.GetData(m_nCh);

	if(!xt || ! yt)
	{
		m_nErr = ERR_DATA;
		return;
	}

	Aurora::SampleCount length = src.GetLength();

	if(length > dst.GetLength())
	{
		m_nErr = ERR_LENGTH_MISMATCH;
		return;
	}

	double rate = src.GetRate();

	if(rate != dst.GetRate())
	{
		m_nErr = ERR_RATE_MISMATCH;
		return;
	}

	// filtro 1 - passa alto 0.7943 Hz
	const double c_1 =  1.0 - exp(-2.0 * M_PI * (0.7943) / rate);
	const double c_1m = 1.0 - c_1;
	// filtro 2 - passa alto 0.7943 Hz
	const double c_2 =  1.0 - exp(-2.0 * M_PI * (0.7943) / rate);
	const double c_2m = 1.0 - c_2;
	// filtro 3 - passa basso 5.684 Hz
	const double c_3 =  1.0 - exp(-2.0 * M_PI * (5.684) / rate);
	const double c_3m = 1.0 - c_3;
	// filtro 4 - passa basso 100 Hz
	const double c_4 =  1.0 - exp(-2.0 * M_PI * (100.0) / rate);
	const double c_4m = 1.0 - c_4;
	// filtro 5 - passa basso 100 Hz
	const double c_5 =  1.0 - exp(-2.0 * M_PI * (100.0) / rate);
	const double c_5m = 1.0 - c_5;

	Aurora::SampleCount k;
	double delta_1 = 0.0;
	double delta_2 = 0.0;
	double A,B,C=0.0, D=0.0, E=0.0;

	for(k = 0; k < length; k++)
	{
		//      primi 2 filtri passa alto in cascata
		A       = xt[k] - delta_1;
		delta_1 = xt[k] * c_1 + delta_1 * c_1m;
		B       = A - delta_2;
		delta_2 = A * c_2 + delta_2 * c_2m;

		//      3 filtri passa basso conclusivi
		C = B * c_3 + C * c_3m;
		D = C * c_4 + D * c_4m;
		E = D * c_5 + E * c_5m;
		yt[k] = Aurora::Sample(1.19 * E); // correzione per rendere il guadagno ad 1 Hz = 0 dB
	}
}

void Aurora::Filter::ITU_P56_Filter(Aurora::AudioTrack& dst, Aurora::AudioTrack& src)
{
	// code by Angelo Farina
	// Filtro passa-banda secondo ITU-P56 (160-5500 Hz)
	double dbF0,dbFq;

	//  filtro 1 - passa alto 110 Hz
	dbF0=110.0; // Hz
	dbFq=0.7;   // Butterworth
	DoFilter(dst, src, HPF, dbF0, dbFq);
	//  filtro 2 - passa alto 90 Hz
	dbF0=90.0;  // Hz
	dbFq=1.5;
	DoFilter(dst, src, HPF, dbF0, dbFq);
	//  filtro 3 - passa basso 6500 Hz
	dbF0=6500.0; // Hz
	dbFq=0.8;
	DoFilter(dst, src, LPF, dbF0, dbFq);
	//  filtro 4 - passa basso 7500 Hz
	dbF0=7500.0; // Hz
	dbFq=1.8;     //era 1.9
	DoFilter(dst, src, LPF, dbF0, dbFq);
	//  filtro 5 - passa banda 1100 Hz
	dbF0=1100.0; // Hz
	dbFq=0.11;
	DoFilter(dst, src, BPF, dbF0, dbFq);

	// Correzione guadagno -0.11 dB
	dst.GaindB(-0.15);
}

void Aurora::Filter::UNI_9916_Filter(Aurora::AudioTrack& dst, Aurora::AudioTrack& src)
{
	// Filtro passa basso a 250 Hz - UNI 9916
	double dbF0,dbFq;

	//  filtro 1 - passa basso 250 Hz
	dbF0=320.0; // Hz
	dbFq=1.0;   // Butterworth
	DoFilter(dst, src, LPF, dbF0, dbFq);
	//  filtro 2 - passa basso 260 Hz
	dbF0=360.0;  // Hz
	dbFq=0.6;
	DoFilter(dst, src, LPF, dbF0, dbFq);
	//  filtro 3 - passa basso 270 Hz
	dbF0=400.0; // Hz
	dbFq=1.1;
	DoFilter(dst, src, LPF, dbF0, dbFq);
	//  filtro 4 - passa basso 280 Hz
	dbF0=500.0; // Hz
	dbFq=0.5;     //era 1.9
	DoFilter(dst, src, LPF, dbF0, dbFq);

	// Correzione guadagno 0.0 dB
	dst.GaindB(0.0);
}

void Aurora::Filter::OctaveFilter(Aurora::AudioTrack& dst,
                                  Aurora::AudioTrack& src,
                                  const double fcb)
{
   // code by Angelo Farina

   double f0, fq;
   // Band filtering with six-poles IIR filter
   // first filter (2 poles)
   f0 = 0.755f * fcb;
   fq = 6.0f;
   DoFilter(dst, src, BPF, f0, fq);
   // second filter (2 poles)
   f0 = fcb;
   fq = 3.0f;
   DoFilter(dst, src, BPF, f0, fq);
   // third filter (2 poles)
   f0 = 1.33f * fcb;
   fq = 6.0f;
   DoFilter(dst, src, BPF, f0, fq);
}

void Aurora::Filter::RemoveMean(Aurora::AudioTrack& track)
{
    // code by Angelo Farina
	Aurora::Sample* data = track.GetData(m_nCh);
	if(!data)
	{
		m_nErr = ERR_DATA;
		return;
	}

	Aurora::SampleCount length = track.GetLength();

    Aurora::Sample mean = 0.0;
    Aurora::SampleCount i;

    for(i = 0; i < length; i++)
    	mean += data[i];

    mean /= Aurora::Sample(length);

    for(i = 0; i <length; i++)
	  data[i] -= mean;

}

Aurora::Filter::Filter(const int ch)
 : m_nErr(ERR_OK), m_nCh(0)
{}

Aurora::Filter::~Filter()
{}


