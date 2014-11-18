#include "stdafx.h"
#include <math.h>
#include <exception>
#include <vector>
#include <iostream>
using namespace std;


namespace Zeta
{
	class DOUBLE_ARRAY
	{
	private:
		std::vector<double> c;
		int s;
	public:
		DOUBLE_ARRAY Matrix_Add(DOUBLE_ARRAY Return, DOUBLE_ARRAY Unitary)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(ReturnSize());
			for(int i = 0; i < ReturnSize(); i++)
			{
				temp.Set(Return[i] + (Unitary[i] + this[i]), i); 
			}
		}
		DOUBLE_ARRAY Matrix_Add(DOUBLE_ARRAY *Return, DOUBLE_ARRAY *Unitary)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(ReturnSize());
			for(int i = 0; i < ReturnSize(); i++)
			{
				temp.Set(Return[i] + (Unitary[i] + this[i]), i); 
			}
		}
		DOUBLE_ARRAY operator+(DOUBLE_ARRAY lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) + lhs.Return(i),i);
			}
			return temp;
		}
		DOUBLE_ARRAY operator-(DOUBLE_ARRAY lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) - lhs.Return(i),i);
			}
			return temp;
		}
		DOUBLE_ARRAY operator*(DOUBLE_ARRAY lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) * lhs.Return(i),i);
			}
			return temp;
		}
		DOUBLE_ARRAY operator/(DOUBLE_ARRAY lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) / lhs.Return(i),i);
			}
			return temp;
		}

		DOUBLE_ARRAY operator+(int& lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) + lhs,i);
			}
			return temp;
		}
		DOUBLE_ARRAY operator-(int& lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) - lhs,i);
			}
			return temp;
		}
		DOUBLE_ARRAY operator*(int& lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) * lhs,i);
			}
			return temp;
		}
		DOUBLE_ARRAY operator/(int& lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) / lhs,i);
			}
			return temp;
		}


		DOUBLE_ARRAY operator+(DOUBLE_ARRAY *lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) + lhs->Return(i),i);
			}
			return temp;
		}
		DOUBLE_ARRAY operator-(DOUBLE_ARRAY *lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) - lhs->Return(i),i);
			}
			return temp;
		}
		DOUBLE_ARRAY operator*(DOUBLE_ARRAY *lhs)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < s; i++)
			{
				temp.Set(Return(i) * lhs->Return(i),i);
			}
			return temp;
		}

		double operator[](int n)
		{
			return c[n];
		}
		
		bool Equals(DOUBLE_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			for(int i = 0; i < s; i++)
			{
				if(!(Return(i) == lhs->Return(i)))
					return false;
			}
			return true;
		}
		bool GreaterThanOrEqualTo(DOUBLE_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(ReturnGreyScale()>=lhs->ReturnGreyScale());
		}
		bool LessThanOrEqualTo(DOUBLE_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(ReturnGreyScale()<=lhs->ReturnGreyScale());
		}
		bool GreaterThan(DOUBLE_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(ReturnGreyScale()>lhs->ReturnGreyScale());
		}
		bool LessThan(DOUBLE_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(ReturnGreyScale()<lhs->ReturnGreyScale());
		}
		bool DoesNotEqual(DOUBLE_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(!(lhs == this));
		}

		DOUBLE_ARRAY operator++()
		{
			for(int i = 0; i < s; i++)
			{
				c[i]++;
			}
			return *this;
		}
		DOUBLE_ARRAY operator--()
		{
			for(int i = 0; i < s; i++)
			{
				c[i]--;
			}
			return *this;
		}

		operator double*() 
		{ return &c[0]; };
		operator double()
		{return ReturnGreyScale();};

		DOUBLE_ARRAY(int size)
		{
			c = vector<double>(size);
			s = size;
		}
		DOUBLE_ARRAY(double val)
		{
			c = vector<double>();
			c.clear();
			c.push_back(val);
			s = 1;
		}
		DOUBLE_ARRAY(double *arr, int size)
		{
			s = size;
			c.clear();
			c.assign(arr, arr+s);
		} //Arr is the buffer to be copied, size is the amount of indicies to be copied

		double Return(int i)
		{
			return c.at(i);
		};
		void Set(double value, int i)
		{
			c.at(i)= value;
		}

		void SetdoubleBuffer(double *To, int size)
		{
			c.assign(To, To+size);
		}
		double ReturnGreyScale() 
		{
			double sum = 0;

			for(int i = 0; i < s; i++){sum += Return(i);};
			return sum / s;
		}
		int ReturnSize() 
		{return s;};
		double Distance(DOUBLE_ARRAY other)
		{
			double sum;
			for(int i = 0; i < s; i++)
			{
				sum += pow(Return(i) - other.Return(i),2);
			}
			return sqrt(sum);
		};
		DOUBLE_ARRAY Step(DOUBLE_ARRAY other, int itteration)
		{
			DOUBLE_ARRAY temp = DOUBLE_ARRAY(s);
			for(int i = 0; i < other.ReturnSize(); i++)
			{
				temp.Set(other.Return(i) - Return(i) ,i);
			};
			return (temp / itteration);
		}

		double *ToDoubleArray() { return &c[0];};

		int SizeOf()
		{
			return sizeof(this->c);
		}
	};
	class FLOAT_ARRAY
	{
	private:
		std::vector<float> c;
		int s;
	public:
		void Add(FLOAT_ARRAY lhs)
		{
			for(int i = 0; i < s; i++)
			{
				Set(Return(i) + lhs.Return(i), i);
			}
		}
		void Subtract(FLOAT_ARRAY lhs)
		{
			for (int i = 0; i < s; i++)
			{
				Set(Return(i) - lhs.Return(i), i);
			}
		}
		void Multiply(FLOAT_ARRAY lhs)
		{
			for (int i = 0; i < s; i++)
			{
				Set(Return(i) * lhs.Return(i), i);
			}
		}
		void Divide(FLOAT_ARRAY lhs)
		{
			for (int i = 0; i < s; i++)
			{
				Set(Return(i) / lhs.Return(i), i);
			}
		}


		float operator[](int n)
		{
			return c[n];
		}

		bool Equals(FLOAT_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			for(int i = 0; i < s; i++)
			{
				if(!(Return(i) == lhs->Return(i)))
					return false;
			}
			return true;
		}
		bool GreaterThanOrEqualTo(FLOAT_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(ReturnGreyScale()>=lhs->ReturnGreyScale());
		}
		bool LessThanOrEqualTo(FLOAT_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(ReturnGreyScale()<=lhs->ReturnGreyScale());
		}
		bool GreaterThan(FLOAT_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(ReturnGreyScale()>lhs->ReturnGreyScale());
		}
		bool LessThan(FLOAT_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(ReturnGreyScale()<lhs->ReturnGreyScale());
		}
		bool DoesNotEqual(FLOAT_ARRAY *lhs)
		{
			if(lhs->ReturnSize() != s)
				return false;
			return(!(lhs == this));
		}

		FLOAT_ARRAY operator++()
		{
			for(int i = 0; i < s; i++)
			{
				c[i]++;
			}
			return *this;
		}
		FLOAT_ARRAY operator--()
		{
			for(int i = 0; i < s; i++)
			{
				c[i]--;
			}
			return *this;
		}

		operator float*() 
		{ return &c[0]; };
		operator float()
		{return ReturnGreyScale();};

		FLOAT_ARRAY(int size)
		{
			c = vector<float>(size);
			s = size;
		}
		FLOAT_ARRAY(float val)
		{
			c = vector<float>();
			c.clear();
			c.push_back(val);
			s = 1;
		}
		FLOAT_ARRAY(float *arr, int size)
		{
			s = size;
			c.clear();
			c.assign(arr, arr+s);
		}
		FLOAT_ARRAY()
		{

		};
		void AllocFirst()
		{
			c = vector<float>();
			s = 0;
		}
		void PushBackNull()
		{
			s++;
			c.push_back(*(new float));
		}
		float Return(int i)
		{
			return c.at(i);
		};
		void Set(float value, int i)
		{
			c.at(i)= value;
		}

		void SetfloatBuffer(float *To, int size)
		{
			c.assign(To, To+size);
		}
		float ReturnGreyScale() 
		{
			float sum = 0;

			for(int i = 0; i < s; i++){sum += Return(i);};
			return sum / s;
		}
		int ReturnSize() 
		{return s;};
		float Distance(FLOAT_ARRAY other)
		{
			float sum = 0;
			for(int i = 0; i < s; i++)
			{
				sum += pow(Return(i) - other.Return(i),2);
			}
			return sqrt(sum);
		};
		FLOAT_ARRAY Step(FLOAT_ARRAY other, int itteration)
		{
			FLOAT_ARRAY temp = FLOAT_ARRAY(s);
			for(int i = 0; i < other.ReturnSize(); i++)
			{
				temp.Set(other.Return(i) - Return(i) ,i);
			};
			return (temp / itteration);
		}

		float *ToFloatArray() { return &c[0];};

		int SizeOf()
		{
			return sizeof(this->c);
		}
	};
	class COLOR4 : public DOUBLE_ARRAY
	{
		COLOR4(double r, double g, double b, double a)
			: DOUBLE_ARRAY(4)
		{
			Set_R(r);
			Set_G(g);
			Set_B(b);
			Set_A(a);
		};
		double Get_R(){return Return(0);};
		double Get_G(){return Return(1);};
		double Get_B(){return Return(2);};
		double Get_A(){return Return(3);};

		void Set_R(double Value){Set(Value, 0);}
		void Set_G(double Value){Set(Value, 1);}
		void Set_B(double Value){Set(Value, 2);}
		void Set_A(double Value){Set(Value, 3);}

	};
	class COLOR3: public DOUBLE_ARRAY
	{
		COLOR3(double r, double g, double b)
			: DOUBLE_ARRAY(3)
		{
			Set_R(r);
			Set_G(g);
			Set_B(b);
		};
		COLOR3 operator=( DOUBLE_ARRAY lhs)
		{
			SetdoubleBuffer((double*)lhs, sizeof(lhs) / sizeof(lhs[0]));
		}
		double Get_R(){return Return(0);};
		double Get_G(){return Return(1);};
		double Get_B(){return Return(2);};

		void Set_R(double Value){Set(Value, 0);}
		void Set_G(double Value){Set(Value, 1);}
		void Set_B(double Value){Set(Value, 2);}
	};
	class VECTOR2D : public DOUBLE_ARRAY
	{
	public:
		VECTOR2D(double X, double Y)
			:	DOUBLE_ARRAY(2)
		{
			Set_X(X);
			Set_Y(Y);
		};
		VECTOR2D(double* arr)
			:	DOUBLE_ARRAY(arr, sizeof(arr) / sizeof(arr[0]))
		{
		};
		VECTOR2D(DOUBLE_ARRAY f)
			:	DOUBLE_ARRAY(f.ToDoubleArray(), f.ReturnSize())
		{
		};
		VECTOR2D()
			: DOUBLE_ARRAY(2)
		{};
		const double Get_X(){return Return(0);};
		const double Get_Y(){return Return(1);};

		void Set_X(double Value){Set(Value, 0);}
		void Set_Y(double Value){Set(Value, 1);}
		void createCopy(VECTOR2D *copy)
		{
			copy->Set_X(Get_X());
			copy->Set_Y(Get_Y());
		}
	};
	class VECTOR2DF : public FLOAT_ARRAY
	{
	public:
		VECTOR2DF(float X, float Y)
			: FLOAT_ARRAY(2)
		{
			Set_X(X);
			Set_Y(Y);
		};
		VECTOR2DF(float* arr)
			: FLOAT_ARRAY(arr, sizeof(arr) / sizeof(arr[0]))
		{
		};
		VECTOR2DF(FLOAT_ARRAY f)
			: FLOAT_ARRAY(f.ToFloatArray(), f.ReturnSize())
		{
		};
		VECTOR2DF()
			: FLOAT_ARRAY(2)
		{};
		const float Get_X(){ return Return(0); };
		const float Get_Y(){ return Return(1); };

		void Set_X(float Value){ Set(Value, 0); }
		void Set_Y(float Value){ Set(Value, 1); }
		void createCopy(VECTOR2DF *copy)
		{
			copy->Set_X(Get_X());
			copy->Set_Y(Get_Y());
		}
	};

	struct double_Grid
	{
	public:
		vector<double> *X; 
		vector<double> *Y;
		
	};
	struct BoundingBox
	{
	private:
		int size_x, size_y;
		int m;
	public:
		BoundingBox(int grid_x, int grid_y, int mod)
		{
			size_x = grid_x;
			size_y = grid_y;
			m = mod;
		};
		int Size_X()
		{
			return size_x;
		}
		int Size_Y()
		{
			return size_y;
		}
		int Modulo()
		{
			return m;
		}
		BoundingBox(){};
		int ReturnTotalX(){return this->size_x * m;};
		int ReturnTotalY(){return this->size_y * m;};

		int ReturnGridX() {return size_x;};
		int ReturnGridY() {return size_y;};
	
		VECTOR2D *ReturnGridLocalPosition(int x, int y){return new VECTOR2D(x % m, y % m);}
		VECTOR2D *ReturnGridWorldPosition(int grid_x, int grid_y){ return new VECTOR2D(grid_x * m, grid_y * m); };

		VECTOR2D *ReturnCurrentGrid(int x, int y){ return new VECTOR2D((int)(x / m) / 2, (int) (y / m)); };
		VECTOR2D *ReturnCurrentGrid(int x, int y,int mod){ return new VECTOR2D((int)(double)(x / mod), (int)(double) (y / mod)); };
		
		bool InRange(VECTOR2D Point)
		{
			if(Point.Get_X() >= 0 && Point.Get_Y() >= 0)
				if(Point.Get_X() < ReturnTotalX() && Point.Get_Y() < ReturnTotalY())
					return true;
			return false;
		}
	};
	class Image
	{
	private:
		BoundingBox box;
	public:
		vector<vector<DOUBLE_ARRAY*>> col_array;
		Image(int grid_x, int grid_y, int mod)
		{
			col_array = vector<vector<DOUBLE_ARRAY*>>();
			col_array.resize(grid_x * mod);
			box = BoundingBox(grid_x, grid_y, mod);
		};
		Image(){col_array = vector<vector<DOUBLE_ARRAY*>>();};
		void ClearImage(double *col)
		{
			for(int i = 0; i < box.Size_X() * box.Modulo(); i++)
			{
				col_array.at(i).resize(box.Size_Y() * box.Modulo());
				for(int j = 0; j <box.Size_Y() * box.Modulo(); j++)
				{
					col_array[i][j] = new DOUBLE_ARRAY(col,1);
				}
			}
		};
		void ClearImage(double *col, int size)
		{
			for(int i = 0; i < box.Size_X() * box.Modulo(); i++)
			{
				col_array.at(i).resize(box.Size_Y() * box.Modulo());
				for(int j = 0; j < box.Size_Y() * box.Modulo(); j++)
				{
					col_array[i][j] = new DOUBLE_ARRAY(col,size);
				}
			}
		};
		void ClearImage()
		{
			for(int i = 0; i < box.Size_X() * box.Modulo(); i++)
			{
				col_array.at(i).resize(box.Size_Y() * box.Modulo());
				for(int j = 0; j < box.Size_Y() * box.Modulo(); i++)
				{
					col_array[i][j] = new DOUBLE_ARRAY(col_array[i][j]->ReturnSize());
				}
			}
		};

		VECTOR2D *ReturnGridLocalPosition(int x, int y){return box.ReturnGridLocalPosition(x,y);};
		VECTOR2D *ReturnGridWorldPosition(int grid_x, int grid_y){ return box.ReturnGridWorldPosition(grid_x, grid_y); };

		VECTOR2D *ReturnCurrentGrid(int x, int y){ return box.ReturnCurrentGrid(x, y); };
		VECTOR2D *ReturnCurrentGrid(int x, int y,int mod){ return box.ReturnCurrentGrid(x, y,mod); };

		double Delta(VECTOR2D start, VECTOR2D end, int itterations)
		{
			DOUBLE_ARRAY arr = start.Step(end, itterations - 1);
			VECTOR2D *s = new VECTOR2D(arr.Return(0), arr.Return(1));
			double sum = 0;
			for(int i = 0; i < itterations; i++)
			{
				DOUBLE_ARRAY t = DOUBLE_ARRAY((start + (*s * i)));

				sum += col_array[(int)t.Return(0)][(int)t.Return(1)]->ReturnGreyScale();
			};
			return sum / itterations;
		};

		bool InRange(VECTOR2D Point)
		{
			return box.InRange(Point);
		};
		void Check()
		{
			for(int i = 0; i < ReturnTotalY(); i++)
			{
				int j = 0;
				for(j = 0; j < ReturnTotalX(); j++)
				{
					if((col_array[j][i]->Return(0)  > 0)&&(col_array[j][i]->Return(0) < 0.00000000000001)){
						DOUBLE_ARRAY *v = new DOUBLE_ARRAY(col_array[j][i]->Return(0) * 10000000000000);
						col_array[j][i] = v;
					}
					col_array[j][i]->Set(abs((double)((int) (col_array[j][i]->Return(0)*1000))/1000), 0);

				}
			}
		}
		void Check(int index)
		{
			for(int i = 0; i < ReturnTotalY(); i++)
			{
				int j = 0;
				for(j = 0; j < ReturnTotalX(); j++)
				{
					try
					{
						if((col_array[j][i]->Return(index)  > 0)&&(col_array[j][i]->Return(index) < 0.00000000000001)){
							DOUBLE_ARRAY *v = new DOUBLE_ARRAY(col_array[j][i]->Return(index) * 10000000000000);
							col_array[j][i] = v;
						}
						col_array[j][i]->Set(abs((double)((int) (col_array[j][i]->Return(index)*1000))/1000), index);
					}
					catch(exception e)
					{
						int s = col_array[j][i]->ReturnSize();
						double *arr = col_array[j][i]->ToDoubleArray();
						arr[index] = 0;
						col_array[j][i] = new DOUBLE_ARRAY(arr, s + 1);

					};


				}
			}
		}
		void Print()
		{
			for(int i = 0; i < ReturnTotalY(); i++)
			{
				int j = 0;
				for(j = 0; j < ReturnTotalX(); j++)
				{
					std::cout<<col_array[j][i]->Return(0) << "\t ";
				}
				cout<<endl;
			}
		}
		void Print(int index)
		{
			for(int i = 0; i < ReturnTotalY(); i++)
			{
				int j = 0;
				for(j = 0; j < ReturnTotalX(); j++)
				{
					try
					{
						std::cout<<col_array[j][i]->Return(index) << "\t ";
					}
					catch(exception e)
					{
						std::cout<<"0" << "\t ";
					};
				}
				cout<<endl;
			}
		}
		int Get_Modulo(){ return box.Modulo(); };

		int ReturnTotalX(){return box.ReturnTotalX();};
		int ReturnTotalY(){return box.ReturnTotalY();};

		int ReturnGridX() {return box.ReturnGridX();};
		int ReturnGridY() {return box.ReturnGridY();};

		BoundingBox ReturnBox() { return box; };

		void PSET(DOUBLE_ARRAY *col, int size, int x, int y) { col_array[x][y] = new DOUBLE_ARRAY(col->ToDoubleArray(),size); };
		void PSET(DOUBLE_ARRAY *col, int x, int y) { col_array[x][y] = new DOUBLE_ARRAY(col->ToDoubleArray(),1); };

		DOUBLE_ARRAY *PGET(int x, int y) {return col_array[x][y];};
		DOUBLE_ARRAY *PGET(VECTOR2D *Pos) {return col_array[(int)Pos->Get_X()][(int)Pos->Get_Y()];};

		DOUBLE_ARRAY D_PGET(int x, int y) {return *col_array[x][y];};
		DOUBLE_ARRAY D_PGET(VECTOR2D *Pos) {return *col_array[(int)Pos->Get_X()][(int)Pos->Get_Y()];};
		
		void Resize(int size_x, int size_y, int Modulo)
		{
			col_array.resize(size_x * Modulo);
			box = BoundingBox(size_x, size_y, Modulo);
	
		};
	};
	
};