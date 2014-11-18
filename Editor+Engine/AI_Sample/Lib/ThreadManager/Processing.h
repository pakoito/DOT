#ifndef THREADING_PROCESSING_HEADER
#define THREADING_PROCESSING_HEADER
#include "../Utility.h"
#include <ppl.h>
using namespace concurrency; 
namespace Zeta{
	class Process
	{

	protected:
		bool Active,Multithread, GPU;
	public:
		int Height, Width;
		int mod_x, mod_y;
		int m;
		Image Output;
		Process()
		{
			Height = 0;
			Width = 0;
			m = 1;
			Multithread = GPU = Active = false;
		}
		Process(int size_x, int size_y, int Modulo)
		{ 
			Height = size_y * Modulo;
			Width = size_x * Modulo;
			Multithread = Active = true;
			m = Modulo;

			mod_x = size_x / Modulo;
			mod_y = size_y / Modulo;

			if(((double)size_x / Modulo) != mod_x)
			{ throw "Error, the size of double_Pattern must be divisible by " + Modulo; };

			Output = Image(mod_x * 3, mod_y * 3, 1);
			Output.ClearImage(new double(0),1);
		};
		void SetThreadingCapabilities(bool MutliThread , bool HasGPUSupport )
		{
			Multithread = MutliThread;
			GPU = HasGPUSupport; 
		}

		Image ReturnDeRefOutput(){ return Output; };
		Image *ReturnOutput()
		{
			return &Output;
		}

		void SetParent(Process *newParent) { this->ParentTask = newParent; };

		bool CanMultiThread()
		{
			return Multithread;
		};
		bool IsGPUCapable()
		{
			return GPU;
		}

		virtual int Compute_CPU(Image*) = 0;
		virtual int Compute_CPU(Image**, int size_x, int size_y) = 0;
		
		void PSET(int x, int y, DOUBLE_ARRAY *da)
		{
			Output.col_array[x][y] = da;
		};
		DOUBLE_ARRAY *PGET(int x, int y)
		{
			return Output.PGET(x, y);
		};
		DOUBLE_ARRAY *PGET(VECTOR2D *pos)
		{
			return Output.PGET(pos);
		}
		void Parent_PSET(int x, int y, DOUBLE_ARRAY *da)
		{
			ParentTask->Output.col_array[x][y] = da;
		}
		DOUBLE_ARRAY *Parent_PGET(int x, int y)
		{
			return ParentTask->PGET(x, y);
		};
		DOUBLE_ARRAY *Parent_PGET(VECTOR2D *pos)
		{
			return ParentTask->PGET(pos);
		}
	private:
		
		Process *ParentTask;
	};
	class UnitaryProcess : public Process
	{
		Image Matrix, Return;
		int size_x, size_y;
		UnitaryProcess(int size_x, int size_y, int Modulo = 1)
			: Process(size_x, size_y, Modulo)
		{
			this->size_x = size_y;
			this->size_y = size_y;
		}
		UnitaryProcess()
			: Process()
		{
		}
		void SetMatrix(Image newm)
		{
			Matrix = newm;
		}
		int Compute_CPU(Image *input) //This should be optimized to use a block method
		{
			for(int i = 0; i < size_x; i++)
			{
				for(int j = 0; j < size_y; j++)
				{
					for(int k = 0; k < size_x; k++)
						Return.PSET(&input->PGET(j,k)->Matrix_Add(Return.PGET(i,k), Matrix.PGET(i,j)),i,k);
				}
			}
		}
	};
	typedef std::vector<Process*> Chain;
	typedef Chain Processes;
	struct FrequencyIndex
	{
		int Freq, Index;
	};
	class ResonanceManager : public Process
	{
	private:
		int size;
		Chain p;
		vector<FrequencyIndex> Frequencies;
		Image *Final;
	protected:
		void Final_PSET(int x, int y, DOUBLE_ARRAY *da)
		{
			Final->col_array[x][y] = da;
		}
		DOUBLE_ARRAY *Final_PGET(int x, int y)
		{
			return Final->PGET(x, y);
		};
		DOUBLE_ARRAY *Final_PGET(VECTOR2D *pos)
		{
			return Final->PGET(pos);
		}
	public:
		Process *GetProcess(int i)
		{
			return p.at(i); 
		}
		ResonanceManager()
			: Process()
		{

			p = Chain();
			size = 0;
		}

		ResonanceManager(Chain Transform, int size_x, int size_y, int Modulo)
			: Process(size_x, size_y, Modulo)
		{
			p = Chain();
			p = Transform;
			size = p.size();
			Frequencies = vector<FrequencyIndex>();
			Frequencies.resize(size);
			for(int i = 0; i < size; i++)
			{
				p.at(i)->SetParent(this);
			};
		}

		int Compute_CPU(Image* Input)
		{
			parallel_for(size_t(0), size_t(size), [&](size_t i)
			{
				Process *cur = GetProcess(i);
				FrequencyIndex temp;
				temp.Freq =  cur->Compute_CPU(Input);
				temp.Index = i;
				Frequencies.assign(i,temp);
			});
			return 0;
		}

		virtual int Compute_CPU(Image**, int size_x, int size_y) = 0; //Not very useful in actuallity for this class
		virtual int CombineBuffers() = 0;

		
	};
	
	class ChainManager
	{
	public:
		ChainManager()
		{
			count = 0;
			p_Chain = new std::vector<Chain>();
		}
		void AddNewChain(Chain ps)
		{
			p_Chain->resize(count + 1);
			p_Chain->at(count) = ps;
			count++;

		}	
		std::vector<Processes> *ReturnChain()
		{
			return p_Chain;
		}
		std::vector<Image> Run( _In_ Image **Input, int start, int end)
		{

			std::vector<Image> Out = std::vector<Image>();
			Out.resize(end - start);
			parallel_for(size_t(start), size_t(end), [&](size_t i)
			{
				Processes cur = ReturnChain()->at(i);
				Out[i] = *Input[i];
				for(int j = 0; j < cur.size();j++)
				{

					if(cur[j]->Compute_CPU(&Out[i]) != -1)
						Out[i] = cur[j]->ReturnDeRefOutput();
					else{
						throw new exception("Error: Parallel tasking is not supported by Process Chain: " + i);
					}
				}
			});
			return Out;
		}
		std::vector<Image> CombineBuffers( _In_ Image **Input, int start, int end,int size_x, int size_y)
		{
			std::vector<Image> Out = std::vector<Image>();
			Out.resize(end - start);
			parallel_for(size_t(start), size_t(end), [&](size_t i)
			{
				Processes cur = ReturnChain()->at(i);
				Out[i] = *Input[i];
				if(cur[0]->Compute_CPU(Input, size_x, size_y)!= -1)
					Out[i] = cur[0]->ReturnDeRefOutput();
				else{
						throw new exception("Error: Parallel resonance expansion is not supported by Process Chain: " + i);
				}
			});
			return Out;
		};
	private:
		std::vector<Chain> *p_Chain;
		int count;

	};
};

#endif