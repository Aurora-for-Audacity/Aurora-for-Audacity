% sweep generator
clear;

fs = 48000;    % Hz
f1 = 20;       % Hz
f2 = 20000;    % Hz
duration = 10; % s

w1 = 2 * pi * f1;
w2 = 2 * pi * f2;
lnW2overW1 = log(f2 / f1);
lnRatio = lnW2overW1 / duration;

Tsamples = duration * fs;
output0 = zeros(Tsamples, 1);
frq0 = zeros(Tsamples, 1);

for k = 1 : Tsamples
    arg = (w1 / lnRatio) * (exp(k * lnRatio / fs) - 1);
    frq0(k) = arg / (2 * pi);
    output0(k) = sin(arg);
end

subplot(6,1,1);
plot(output0);

subplot(6,1,2);
plot(frq0);

Noctave = log(f2/f1) / log(2);
N       = Tsamples / Noctave;
w  = (w2 / fs) * 2 ^(-Noctave) * (N / log(2));

output1 = zeros(Tsamples, 1);
frq1 = zeros(Tsamples, 1);

for k = 1 : Tsamples
    % s   = std::sin(w * (std::pow(2.0f, ii /flN) - 1.0f));    
    arg = w * (2 ^ (k/N) - 1);
    frq1(k) = arg / (2 * pi);
    output1(k) = sin(arg);
end

subplot(6,1,3);
plot(output1);

subplot(6,1,4);
plot(frq1);

% Fons' aliki
%
%    // _k0 -> fade in    (samples)
%    // _k1 -> sweep time (samples)
%    // _k2 -> fade out   (samples)
%
%    int     i;
%    double  a, b, g, q, q0, p, r, x;
%    float   *s1, *s2;

%    b = log (_fmax / _fmin) / _k1;
%    a = _fmin / (b * _rate);
%    r = 0.5 * a * (_fmax / _fmin) * (_k1 + 0.5 * (_k0 + _k2)) / (b * _k1);
%    s1 = _impd.data (0);
%    s2 = _impd.data (1) + 2 * (_impd.n_fram () - 1);
%    q0 = a * exp (-b * _k0);
%    for (i = -_k0; i < _k1 + _k2; i++)
%    {
%        if      (i < 0)   g = cos (0.5 * M_PI * i / _k0);
%        else if (i < _k1) g = 1.0;
%        else              g = sin (0.5 * M_PI * (_k1 + _k2 - i) / _k2);
%	     q = a * exp (b * i);
%        p = q - q0;
%	     x = g * sin (2 * M_PI * (p - floor (p)));
%	     *s1 = (float)(x);
%        *s2 = (float)(x * q / r);
%        s1 += 2;
%        s2 -= 2;
%    }

k0 = 0;
k1 = Tsamples;
k2 = 0;
fmax = f2;
fmin = f1;
rate = fs;
b = log(fmax / fmin) / k1;
a = fmin / (b * rate);
r = 0.5 * a * (fmax / fmin) * (k1 + 0.5 * (k0 + k2)) / (b * k1);
q0 = a * exp(-b * k0);

output2 = zeros(k1, 1);
frq2 = zeros(k1, 1);

for k = 1 : k1
    q = a * exp(b * k);
    p = q - q0;
    output2(k) = sin(2 * pi * (p - floor(p)));
    frq2(k) = p - floor(p);
end

subplot(6,1,5);
plot(output2);

subplot(6,1,6);
plot(frq2);

audiowrite('sweep.wav', output2, fs, 'BitsPerSample', 32);


