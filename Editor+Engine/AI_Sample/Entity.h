/*
Degrees of opination:
Lightly Opinionated-
"Oh, I guess that sort sucks"
Medium Opinionated-
"Now its starting to get on my nerves"
-Nothing really changes. NPCs are just talking about the issue more often, perhaps trying to strick conversation with the player
Heavily Opinionated-
"You know, I should try to do something about this"
-Possible offshots: Can set up political parties, or recruit the player's help
Deathly Opinionated-
"I hate this so much I could rip off someone's head"
-Self explanitory. Could result from no desire to form a political party, or a rebelious act. Can lead to battles
*/
#ifndef ENTITY_HEADER
#define ENTITY_HEADER
#include "stdafx.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>  
#include <time.h> 
#include <memory>
#include "HelperFunctions.h"
#include <map>

class Entity //This is our entity
{
protected:
	bool Death; //Unload on game restart

	
	short Lock; //Use for actions that take a particular amount of time

	std::shared_ptr<Advertisement> Queue; //What add are we gonna do next?

private:
	void ExecuteNegativeEffects(int type,int Delta)
	{
		Attributes[type]->Value -= Delta;
	}
public:
	short Power; //The entity's rank in society
	std::string Link;
	int LinkNumber; //How many others have inherited?
	std::string Name;
	std::vector<EntityAttribute*> Attributes;
	std::vector<int> UsedAttributes;
	std::vector<int> ViableAds;
	std::vector<std::shared_ptr<Advertisement>> SelfAdvertisment;
	std::vector<std::shared_ptr<Advertisement>> BroadcastedAdvertisement;
	float WeighedSum;
	short ReturnLock()
	{
		return this->Lock;
	}
	void IncLock()
	{
		Lock--;
	};
	std::vector<EntityAttribute*>  getGoals()
    {
        return Attributes;
    }

	std::vector<EntityAttribute*> * getGoalsPtr()
    {
        return &Attributes;
    }
	Entity()
	{
		Power = 0;
		ViableAds = std::vector<int>();
		UsedAttributes = std::vector<int>();
		Attributes = std::vector<EntityAttribute*>();
		Death = false; 
		Lock = 0;
		WeighedSum = 1;
	};
	Entity(std::string Name)
	{
		ViableAds = std::vector<int>();
		Attributes = std::vector<EntityAttribute*>();
		UsedAttributes = std::vector<int>();
		Death = false;
		Lock = 0;
		WeighedSum = 1;
	};
	VECTOR2DF Position;
	short ReturnRank() //The closer this is to 0, the higher the NPC's rank
	{
		return Power;
	}
	short ReturnLockVal()
	{
		return this->Lock;
	};
	void LockForTurns(std::shared_ptr<Advertisement> Next)

	{
		int count = (int)(((float)Next.get()->TimeConstant + 1) * (1 / WeighedSum));
		Lock = count;
		this->Queue = Next;
	};
	bool IsLocked()
	{
		return (Lock > 1);
	}
	void CheckDead()
	{
		for (int i = 0; i < (int)Attributes.size(); i++)
		{
			if (Attributes.at(i)->Value < 0)
				Death = true;
		};
		Death = false;
	}
	bool IsDead()
	{
		return Death;
	};
	int Update() //Returns 0 when the entity is still locked, returns 1 when lock ends.
	{
		Lock -= 1;
		//Other update code goes here

		return 0;
	};

	void Tick()
	{
		for (int i = 0; i < (int)Attributes.size(); i++)
		{
			Attributes[i]->Value -= Attributes[i]->Delta;
		};
	};
	void ExecuteAdvertisment(std::shared_ptr<Advertisement> Ad)
	{
		auto Adv = Ad.get();
		float DeltaVal = 0;
		for (int i = 0; i < (int)Ad.get()->positive_effects.size(); i++)
		{
			int LocalIndex = UsedAttributes.at(Ad.get()->positive_effects.at(i).Index);
			float DeltaTemp = Attributes[LocalIndex]->Value;
			Attributes[LocalIndex]->Value += Adv->positive_effects.at(i).DiminishingReturn;
			if (Attributes[LocalIndex]->Value > Attributes[LocalIndex]->grad.Max)
				Attributes[LocalIndex]->Value = Attributes[LocalIndex]->grad.Max;
			DeltaTemp = (Attributes[LocalIndex]->Value - DeltaTemp) / Adv->positive_effects.at(i).DiminishingReturn;
			DeltaVal += DeltaTemp;
		}
		DeltaVal /= (int)Ad.get()->positive_effects.size();
		if (DeltaVal != 0)
		{
			for (int i = 0; i < (int)Ad.get()->negative_effects.size(); i++)
			{
				int LocalIndex = UsedAttributes.at(Ad.get()->negative_effects.at(i).Index);
				Attributes[LocalIndex]->Value -= DeltaVal * Adv->negative_effects.at(i).DiminishingReturn;
			}
		}
		int itt;
		int count = (int)(((float)Ad.get()->TimeConstant + 1) * (1 / WeighedSum));
		for (itt = 0; itt < count; itt++) //Update for how many times it takes to complete this task
			Tick();
		Ad.get()->EndUse();
		CheckDead();
	};
	void ExecuteQueue()
	{
		ExecuteAdvertisment(Queue);
		PreAdUpdate();
	}
	string ReturnNextName()
	{
		return this->Queue.get()->Name;
	}
	void DeleteAttribute(std::string Name)
	{
		int i;
		for (i = 0; i < (int)Attributes.size(); i++)
		{
			if (Attributes.at(i)->Name == Name)
			{
				Attributes.erase(Attributes.begin() + i); break;
			}
		}
		int j;
		for (j = 0; j < (int)UsedAttributes.size(); j++)
		{
			if (i == UsedAttributes.at(j))
			{
				UsedAttributes.at(j) = -1;
				break;
			}
		}

	};
	Entity createUpdateCopy()
	{
		Entity ToReturn;
		ToReturn.SelfAdvertisment = SelfAdvertisment;
		ToReturn.BroadcastedAdvertisement = BroadcastedAdvertisement;
		ToReturn.Attributes = Attributes;
		return ToReturn;
	}
	void createUpdateCopy(Entity *out)
	{
		out->SelfAdvertisment = SelfAdvertisment;
		out->BroadcastedAdvertisement = BroadcastedAdvertisement;

		out->ViableAds = ViableAds;
		out->UsedAttributes = UsedAttributes;
		for (int i = 0; i < (int)Attributes.size(); i++)
		{
			Attributes.at(i)->createCopy(out->Attributes.at(i));
		}

	}
	Entity createCopy()
	{
		Entity ToReturn;
		ToReturn.Position = Position;
		ToReturn.Name = Name;
		ToReturn.SelfAdvertisment = SelfAdvertisment;
		ToReturn.BroadcastedAdvertisement = BroadcastedAdvertisement;
		ToReturn.Attributes = Attributes;
		return ToReturn;
	}
	void createCopy(Entity *out)
	{
		out->Position = Position;
		out->Name = Name;
		out->SelfAdvertisment = SelfAdvertisment;
		out->BroadcastedAdvertisement = BroadcastedAdvertisement;
		out->Attributes = Attributes;
	}
	void PreAdUpdate()
	{
		WeighedSum = 0;
		for (int i = 0; i < (int)Attributes.size(); i++)
		{
			short temp;
			if (Attributes[i]->Value > 0)
				temp = Attributes[i]->grad.Max / (Attributes[i]->Value);
			else
				break;
			WeighedSum += Monitization::TimeAspect(temp, Attributes[i]->Delta, Attributes[i]->Value);
		}
		WeighedSum /= UsedAttributes.size();
	}
	float ComputeInference(Advertisement Ad)
	{
		float Average = 0;
		Average += ReturnScore(*Ad.Infer);
		return Average;
	}
	float ReturnScore(Advertisement Ad)
	{
		int count = (int)(((float)Ad.TimeConstant + 1) * (1 / WeighedSum));
		if (Ad.positive_effects.size() == 0 && Ad.negative_effects.size() == 0)
			return 0;

		float t = 1;
		int avgscore1 = 0;

		//Compute a quick sigmoid
		float m = Ad.TimeConstant;

		m = m / (1 + abs(m));
		m *= WeighedSum;
		bool check = true;
		
		for (int l = 0; l < (int)Ad.positive_effects.size(); l++)
		{
			int LocalIndex = UsedAttributes.at(Ad.positive_effects.at(l).Index);
			auto i = Ad.positive_effects.at(l);
			int score1, score2;
			score1 = score2 = 0;
			score2 = i.DiminishingReturn;
			score1 = Attributes.at(LocalIndex)->Value;
			score2 += score1; //Score2 = Future score

			if (score1 >= Attributes.at(LocalIndex)->grad.Max)
			{
				score2 = Attributes.at(LocalIndex)->grad.Max;
			}
			else
				check = false; //At least once, there was a difference 


			avgscore1 += score2; //Average input value

			t += Monitization::PrecomputedTimeAspectScore(score1, score2, m); //Average positive return val
		}
		if (check) //Everything is maxed out
			return -abs(t);
		avgscore1 /= Ad.positive_effects.size() + 1;
		t /= Ad.positive_effects.size() + 1;

		float k = 1;
		for (int i = 0; i < (int)Ad.negative_effects.size(); i++) //Goes through all the negative side effects and manages them for us
		{
			auto NegCost = Ad.negative_effects.at(i);
			int LocalIndex = UsedAttributes.at(NegCost.Index);
			if (NegCost.DiminishingReturn + (Attributes.at(LocalIndex)->Delta * count) >= Attributes.at(LocalIndex)->Value)
				return 0;

			k += (float)Monitization::AutoWeightedScore_Negative(avgscore1, t, NegCost.DiminishingReturn + (Attributes.at(LocalIndex)->Delta
				* count),this->Attributes[LocalIndex]->Value,this->Attributes[LocalIndex]->grad);
		}
		k /= Ad.negative_effects.size() + 1;

		float inferm = 1;
		if (Ad.Infer != nullptr)
			inferm = ComputeInference(Ad);
		return inferm + ( t * k);
	};
};

#endif