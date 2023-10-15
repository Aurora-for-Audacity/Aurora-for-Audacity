/****************************************************************
 *                                                              *
 *                 Multiple Convolver class                     *
 *                                                              *
 * This class performs N by N matrix filtering using            *
 * partitioned convolution engine.                              *
 *                                                              *
 * ------------------------------------------------------------ *
 * Usage:                                                       *
 *                                                              *
 * 1) Instantiate the class using one of the constructors       *
 *    provided;                                                 *
 * 2) if not done by the constructor, set the filter and the    *
 *    vector using 'SetFilter' and 'SetVector' methods;         *
 * 3) call the 'Convolve' method to start calculations;         *
 * 4) get results using 'GetResultSize' and 'GetResultVector'   *
 *    methods.                                                  *
 * 5) At this point you can load another filter or another      *
 *    vector using 'SetFilter' and/or 'SetVector' and recall    *
 *    'Convolve'.                                               *
 *                                                              *
 * ------------------------------------------------------------ *
 *                                                              *
 * by Simone Campanini 2009                                     *
 *                                                              *
 * ------------------------------------------------------------ *
 *                                                              *
 * multivolver.h                                                *
 *                                                              *
 ***************************************************************/
#ifndef __AURORA_MULTIVOLVER_H__
#define __AURORA_MULTIVOLVER_H__

#include "convolver.h"

namespace Aurora
{
    class Multivolver : public ConvolverBase
    {
        // ***************************** Attributes *******************************
        int m_nRows = 0;
        int m_nCols = 0;
        
        std::vector<Aurora::SamplesVector> m_inputs;
        std::vector<Aurora::SamplesVector> m_outputs;

        Aurora::Matrix<Aurora::SamplesVector> m_filterMatrix;

        std::vector<Sample> m_fbsMax;
        std::vector<SampleCount> m_filterPeakPositions;
        
        bool m_bDone    = false;
        bool m_bAborted = false;
        
    protected:
        struct MatrixOptions
        {
            bool autorange           = false;
            bool firstBlockAutorange = false;
            bool diagonalMode        = false;
            
        } m_matrixOptions;
        
    public: 
        const Aurora::Matrix<Aurora::SamplesVector>& FilterMatrix() const { return m_filterMatrix; }

        int Rows   () const { return m_nRows; }
        int Columns() const { return m_nCols;  }

        // Setup methods
        /**
         * @brief Make room for input data.
         * @param length input data length
         * @param idx index of input vector (-1 means allocate every columns
         * with the same length).
         */
        void SetInputLength(const SampleCount length, 
                            const int idx);

        void SetInputVectorItem (const Aurora::SamplesVector& v,
                                 const int idx);
        
        void SetFilterMatrixItem(Aurora::SamplesVector& v,
                                 const int row, 
                                 const int col);                

        Aurora::SamplesVector& GetInputVectorItem (const int index);
        Aurora::SamplesVector& GetOutputVectorItem(const int index);
        
        const Aurora::SamplesVector& GetInputVectorItem (const int index) const;
        const Aurora::SamplesVector& GetOutputVectorItem(const int index) const;
        
        void GetOutputVectorsInterlaced(Sample *p) const;   // NB: p must be a Sample array of out_length*N size.
        
        virtual void PreProcess () override;
        virtual bool Process    () override;
        virtual void PostProcess() override;

        virtual void InitProgressMeter(const int total) override;
        virtual bool UpdateProgressMeter(const int step) override;
        virtual void DestroyProgressMeter() override { }

    private:
        void AddVector(const SamplesVector& src, SamplesVector& dst);
        
        void ReverseFilter         () override;
        void FindFilterPeakPosition() override;
        void PreserveOutputLength  () override;
        
        bool VectorBoost();                 // uses class gain (SetGain)
        bool VectorBoost(const Sample g);
        bool VectorRemoveDC();
        bool VectorFBAutorange();
        bool VectorAutorange();

    public:
        // --- boolcheckers, get-setters
        MatrixOptions& GetMatrixOptions() { return m_matrixOptions; }
        const MatrixOptions& GetMatrixOptions() const { return m_matrixOptions; }
        
    protected:
        void ResizeFilterAndOutputs(const SampleCount filterLength);
        
        wxString GetCaptionString() const override { return wxString("Aurora Convolver"); }

        // --- 'ctors
        // - N is the filter matrix order (if it is a square matrix);
        // - rows and cols are referred always to the
        //   filter matrix, so if you have a N x M input filter matrix,
        //   you need a M rows input data vector, and you'll obtain an
        //   M rows output data vector
    public:
        virtual void Init(const int rows, 
                          const int cols,
                          const SampleCount in_len, 
                          const SampleCount f_len);

        virtual void Init(const int N,
                          const SampleCount in_len,
                          const SampleCount f_len)
        {
            Init(N, N, in_len, f_len);
        }

        virtual void Destroy();
        
        Multivolver() { }
             
        Multivolver(const int N, 
                    const SampleCount in_len, 
                    const SampleCount f_len); 
        
        Multivolver(const int rows, 
                    const int cols, 
                    const SampleCount in_len, 
                    const SampleCount f_len); 
        ~Multivolver();

    };
} // namespace Aurora

#endif // __AURORA_MULTIVOLVER_H__
