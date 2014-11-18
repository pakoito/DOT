#ifndef HELPERFUNCTIONS_HEADER
#define HELPERFUNCTIONS_HEADER
#include <vector>
#include <math.h>
#include "Lib\Utility.h"
#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
class Entity;
struct Group;

using namespace Zeta;

struct Ownership //For obvious reasons. We could maybe implement theft (?)
{
	bool InUse;
	Entity *e;
};
struct Gradient
{
	int Low, Q1, Q3;
	int Max;
	Gradient(int L = 20, int FirstQuartile = 30, int ThirdQuartile = 50, int Max = 100) //L = lower bound, H = upper bound
	{
		Low = L;
		Q1 = FirstQuartile;
		Q3 = ThirdQuartile;
		this->Max = Max;
	}

};
struct EntityAttribute
{
public:
	EntityAttribute(bool nullset = false)
	{
		if (!nullset)
		{
			this->Name = "Default";
			this->Value = 100;
			this->Delta = 1;
		}
		
	}
	EntityAttribute(string n, int val, int d)
	{
		this->Name = n;
		Value = val;
		Delta = d;
	};
	EntityAttribute(string n, int val, int d, Gradient g)
	{
		this->Name = n;
		Value = val;
		Delta = d;
		grad = g;
	};
	string Name;
	int Value;
	int Delta;
	Gradient grad;
	void SetGradient(Gradient g)
	{
		this->grad = g;
	}
	EntityAttribute createCopy()
	{
		EntityAttribute ea;
		ea.Name = Name;
		ea.Value = Value;
		ea.Delta = Delta;
		ea.grad = grad;
		return ea;
	}
	void createCopy(EntityAttribute *ea)
	{
		ea->Name = Name;
		ea->Value = Value;
		ea->Delta = Delta;
		ea->grad = grad;
	}
};
struct Cost
{
public:
	Cost(std::string StringType,int D )
	{
		this->StringT = StringType;
		this->DiminishingReturn = D;
	}
	Cost()
	{
		StringT = "Default";
		Index = -1;
		DiminishingReturn = 5;
	}
	Cost createCopy()
	{
		return Cost(StringT, DiminishingReturn);
	}
	std::string StringT;
	int Index;
	int DiminishingReturn; //How badly does taking this advertisement hurt us?
};
struct Group
{
public:
	std::vector<int> LTL; //Local to Local
	std::vector<bool> VI_A; //Viable advertisements

	std::vector<Group*> Nodes;

	int MinAdr, MaxAdr;
	int MaxAd,  MinAd;

	void FillAttributes(std::vector<int> NodeAttributes_Global, std::vector<int> NodeAttributes_Local)
	{
		std::sort(NodeAttributes_Global.begin(), NodeAttributes_Global.end());
		MinAdr = NodeAttributes_Global.at(0);
		MaxAdr = NodeAttributes_Global.at(NodeAttributes_Global.size() - 1);

		for (int i = 0; i < MaxAdr - MinAdr; i++)
		{

		}
	};
};
struct GroupManager
{
	Group *MasterNode;
	//Returns the SINGLE node that is linked to the address
	Group *ReturnNodeFromAddress(std::vector<short> Adr)
	{
		Group *temp;
		*temp = *MasterNode;
		for (int i = 0; i < (int)Adr.size(); i++)
		{
			temp = (Group*)temp->Nodes[Adr.at(i)];
		}
		return temp;
	};
	//For chain retreival only
	std::vector<Group*> *ReturnChain(std::vector<short> Adr)
	{
		std::vector<Group*> *Chain = new std::vector<Group*>();
		Chain->push_back(MasterNode);
		for (int i = 0; i < (int)Adr.size(); i++)
		{
			Chain->push_back((Group*)Chain->at(Chain->size() - 1)->Nodes[Adr.at(i)]);
		}
		return Chain;
	};
	//For single address retreival only
	int ReturnLocalAdr(int GlobalAdr, std::vector<short> EntityAdr)
	{
		std::vector<Group*> *Chain = ReturnChain(EntityAdr);
		for (int i = 0; i < (int)Chain->size(); i++)
		{
			if (Chain->at(i)->MaxAdr < GlobalAdr && Chain->at(i)->MinAdr > GlobalAdr)
				return Chain->at(i)->LTL[GlobalAdr - Chain->at(i)->MinAdr];
		}

	}
	//For single advertisement retreival only
	int ReturnLocalAdViability(int AdAdr, std::vector<short> EntityAdr) 
	{
		std::vector<Group*> *Chain = ReturnChain(EntityAdr);
		for (int i = 0; i < (int)Chain->size(); i++)
		{
			if (Chain->at(i)->MaxAd > AdAdr && Chain->at(i)->MinAd < AdAdr)
				return Chain->at(i)->VI_A[AdAdr - Chain->at(i)->MinAd];
		}
	}
	//For multi address retreival only
	std::vector<int> ReturnLocalAdrGroup(std::vector<int> GlobalAdr, std::vector<short> EntityAdr) 
	{
		std::vector<Group*> *Chain = ReturnChain(EntityAdr);
		std::vector<int> LocalAdr = std::vector<int>();
		int i = 0;
		for (int j = 0; j < (int)GlobalAdr.size(); j++) //For every global address we've given
		{
			while (i < (int)Chain->size()) //Make sure that we haven't hit the end of our chain
			{
				if (Chain->at(i)->MaxAdr < GlobalAdr.at(j) && Chain->at(i)->MinAdr > GlobalAdr.at(j)) //Are we within bounds?
				{
					LocalAdr.push_back(Chain->at(i)->LTL[GlobalAdr.at(j) - Chain->at(i)->MinAdr]); //Yes! Add the index to the local adr array
					break;
				}
				i++; //If it wasn't found, go to the next chain link. Otherwise assume that our chain link is in here
			}
		}
		return LocalAdr;
	};
	//Returns all local addresses for the entity specified. Returns a highly expanded vector. Needed for GPGPU
	std::vector<int> ReturnAttributeChain(std::vector<short> EntityAdr)
	{
		std::vector<Group*> *Chain = ReturnChain(EntityAdr);
		std::vector<int> LocalAdr = std::vector<int>();
		for (int i = 0; i < (int)Chain->size(); i++)
		{
			int min, max;
			min = Chain->at(i)->MinAdr;
			max = Chain->at(i)->MaxAdr;

			for (int j = min; j < max; j++)
			{
				//Push back value
				LocalAdr.push_back(Chain->at(i)->LTL.at(j));
			}

			if (i != (Chain->size() - 1)) //We aren't on the last chain
			{
				for (int j = max; j < Chain->at(i + 1)->MinAdr; j++)
				{
					LocalAdr.push_back(-1); //So push back padding
				}
			}
		}
		return LocalAdr;
	};
	//Slower than ReturnChainSizeInBytes_Compressed since the chain must be itterated through in a fully compressed state. Provides a more accurate result
	int ReturnChainSizeInBytes_HyperCompressed(std::vector<short> Adr)
	{
		std::vector<Group*> *Chain = ReturnChain(Adr);
		int size = 0;
		for (int i = 0; i < (int)Chain->size(); i++)
		{
			for (int j = 0; j < (int)Chain->at(i)->LTL.size(); j++)
			{
				if (Chain->at(i)->LTL.at(j) != -1)
					size += 4;
			}
		}
		return size;
	};
	//Returns the amount of (Attribute Addresses + Padding) * sizeof(int). Does not fully decompress the chain, and therefore is only to be used exclusively. Fastest size gauge, with medium accuracy
	int ReturnChainSizeInBytes_Compressed(std::vector<short> Adr)
	{
		std::vector<Group*> *Chain = ReturnChain(Adr);
		int size = 0;
		for (int i = 0; i < (int)Chain->size(); i++)
		{
			size += (int)Chain->at(i)->LTL.size() * 4;
		}
		return size;
	};
	//Slowest size gauge. Required for GPGPU
	int ReturnChainSizeInBytes_Uncompressed(std::vector<short> Adr)
	{
		std::vector<int> vec = ReturnAttributeChain(Adr);
		return vec.size() * 4;
	};
};
class Advertisement //Eventually we can include more variables. Like make advertisements faction owned
{
public:
	Advertisement()
	{
		TimeConstant = 1;
		negative_effects = std::vector<Cost>();
		positive_effects = std::vector<Cost>();
		Name = "Default";
		UsableByMoreThanOne = true;
		MyOwner.InUse = false;
		OwnerSet = false;
		Infer = nullptr;
	};
private:
	Ownership MyOwner;
public:

	void SetOwner(Entity *e)
	{
		this->MyOwner.e = e;
		OwnerSet = true;
	};
	Entity *GetOwner() //Use this with "World"
	{
		if (OwnerSet)
			return MyOwner.e;
		return nullptr;
	}
	void BeginUse()
	{
		MyOwner.InUse = true;
	}
	void EndUse()
	{
		MyOwner.InUse = false;
	}
	bool IsInUse()
	{
		return this->MyOwner.InUse;
	};
	Advertisement createCopy()
	{
		Advertisement ToReturn;
		if (Position != NULL)
			ToReturn.Position = Position;
		ToReturn.Name = Name;
		ToReturn.negative_effects = negative_effects;
		ToReturn.positive_effects = positive_effects;
		ToReturn.OwnerSet = OwnerSet;
		ToReturn.UsableByMoreThanOne = UsableByMoreThanOne;
		ToReturn.MyOwner = MyOwner;
		if (Infer != nullptr)
			Infer->createCopy(ToReturn.Infer);
		return ToReturn;
	};
	void createCopy(Advertisement *out)
	{
		out = new Advertisement();
		if (Position != NULL)
			out->Position = Position;
		out->Name = Name;
		out->negative_effects = negative_effects;
		out->positive_effects = positive_effects;
		out->OwnerSet = OwnerSet;
		out->UsableByMoreThanOne = UsableByMoreThanOne;
		out->MyOwner = MyOwner;
		if (Infer != nullptr)
			Infer->createCopy(out->Infer);
	}
	void createUpdateCopy(Advertisement *out)
	{
		out->negative_effects = negative_effects;
		out->positive_effects = positive_effects;
		out->OwnerSet = OwnerSet;
		out->UsableByMoreThanOne = UsableByMoreThanOne;
		out->MyOwner = MyOwner;
		out->TimeConstant = TimeConstant;
		if (Infer != nullptr)
			Infer->createCopy(out->Infer);
	}
	bool UsableByMoreThanOne; //More than one NPC can use this advertisment
	bool OwnerSet;
	short TimeConstant; //Obviously writing a novel takes more time than eating a sandwich. No matter how healthy you are (I fucking swear I sound insane)
	string Name;
	VECTOR2DF Position; //How far is this advertisement from the NPC?
	std::string Link;
	int LinkNumber;
	int LinkAddress;
	std::vector<Cost> negative_effects; //How badly does this effect us?
	std::vector<Cost> positive_effects; //What good does this do for us?
	Advertisement *Infer;
};
static int binarySearch(std::vector<float> arr, int value, int min, int max){
	int pos = -1;

	while (max >= min && pos == -1) {
		int mid = min + (max - min) / 2;

		if (arr[mid]== value){
			pos = mid;
		}
		else if (arr[mid] < value){
			min = mid + 1;
		}
		else if (arr[mid] > value){
			max = mid - 1;
		}

	}
	return pos;
};
struct Monitization //Contains a set of helper functions for assisting the entity to figure out what helps it the most in its current eco state
{
	/*
	Erf positive is use for tasks with heigh weights. Tanh is slightly lower weighted than Erf. XPlusAbsX is the lowest ranked
	*/
	static float Attune(float input) //Is an increase from 30% to 60% worth just as much as an increase from 60% to 90%?
	{
		return 10 / input;
	};
	static float TimeAspect(float inputscore, int Delta, int EntityValue) //How much time is this process going to take? Are we going to die completing it?
	{
		inputscore *= Delta; //How much food do we need during this time?
		inputscore = 1 - (inputscore / EntityValue); //The more food it takes to complete a task, the lower timeaspect becomes
		return inputscore;
	};
	static float CostAttune_Erf_Positive(int Cost, int EntityValue) //This function is a sigmoid graph. At 0 cost, this function returns 1. At max cost, this function returns near 0
	{
		
		if (Cost > EntityValue)
			return -1;
		float var = (((float)Cost / (float)EntityValue) * 4) - 2; //This is what we feed into Error Function (http://mathworld.wolfram.com/Erf.html)
		var = (float)erf((M_PI / 2) * var);
		var += 1;
		var = 1 - var;

		//The higher the cost goes, the closer this value gets to 0. It never becomes negative. Look at the wolfram link for more information. All that is done here is compressions and scaling. (Just so the value returned is (0,1])
		return var;
	}
	static float CostAttune_Tanh_Positive(int Cost, int EntityValue)
	{
		if (Cost > EntityValue)
			return -1;
		float var = (((float)Cost / (float)EntityValue) * 4) - 2;

		var = tanh(var);
		var += 1;
		var = 1 - var;

		return var;
	}
	static float CostAttune_XPlusAbsX_Positive(int Cost, int EntityValue)
	{
		if (Cost > EntityValue)
			return -1;
		float var = (((float)Cost / (float)EntityValue) * 4) - 2;

		var = var / (1 + abs(var));
		var += 1;
		var = 1 - var;

		return var;
	};
	static double AutoWeightedScore_Negative(int L, float inputscore, int Cost, int EntityValue, Gradient grad = Gradient(20, 30, 50, 60)) //L = score1 (The value we're trying to lerp) This is also quite overpowered
	{

		if (L <= grad.Low) //We're only using Erf
		{
			return (inputscore * CostAttune_Erf_Positive(Cost, EntityValue));
		}
		else if (L <= grad.Q1)
		{
			float t = ((float)grad.Q1 + (float)grad.Q3) / (float)2 - (float)L; //Distance from the First Quartile point
			if (t < 1)
				t = 1;
			t = ((grad.Q3 - grad.Q1) - t) / (grad.Q3 - grad.Q1); //The further the number is from the First Quartil, the smaller it gets. We're comparing against the interquartile range
			return inputscore* ((1 - t) * CostAttune_Tanh_Positive(Cost, EntityValue)) + ((t) * CostAttune_Erf_Positive(Cost, EntityValue));
		}
		else if (L <= grad.Q3)
		{
			return (inputscore * CostAttune_Tanh_Positive(Cost, EntityValue));
		}
		return (inputscore * CostAttune_XPlusAbsX_Positive(Cost, EntityValue));
	}
	static float StaticWeightedScore_Negative(int L, float inputscore, int Cost, int EntityValue)
	{
		if (L< 20) //It needs to be weighed highly
			return Monitization::CostAttune_Erf_Positive(Cost, EntityValue);
		else if (L < 50) //Weighed in a medium matter
			return Monitization::CostAttune_Tanh_Positive(Cost, EntityValue);
		else //Unweighed
			return Monitization::CostAttune_XPlusAbsX_Positive(Cost, EntityValue);
	}
	//Feel free to add any more models you want :D We max out at about 40 models or so, so no need to hold back
	
	static float Score(int input1, int input2, int Delta, int EntityValue, float Completion) //Provides your base score. This is just an upper level for Attune and TimeAspect. Does nothing differently than managing attune and time aspect individually
	{
		float temp = Monitization::Attune((float)input1) - Monitization::Attune((float)input2);
		temp *= Completion;
		return TimeAspect(temp, Delta, EntityValue);
	}
	static float PrecomputedTimeAspectScore(int input1, int input2, float Pre)
	{
		float temp = Monitization::Attune((float)input1) - Monitization::Attune((float)input2);
		return temp / Pre;
	};
};
#endif