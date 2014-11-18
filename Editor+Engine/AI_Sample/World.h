#ifndef WORLD_HEADER
#define WORLD_HEADER
#include "Entity.h"
struct AttributeRef
{
public:
	AttributeRef(std::string Name)
	{
		this->Name = Name;
		Amount = 1;
	}
	AttributeRef(){};
	std::string Name;
	int Amount;

	static bool Compare (AttributeRef i, AttributeRef j) { return i.Amount < j.Amount; };
};
typedef std::vector<std::shared_ptr<Advertisement>> AdList;
typedef std::vector<std::shared_ptr<Entity>> EntityList;
typedef std::vector<AttributeRef> AttributeList;
struct World //This is where our compiling and data management happens! Wooo
{
	AdList *Ads;
	EntityList *Entities;
	AttributeList *Attributes;

	AdList *SimulationAds;
	EntityList *SimulationEntities;

	EntityAttribute *nullatr;

	GroupManager *ClusterManager;


public:
	bool ExternUpdate;
	bool ExternLoad;
	double ScoreNum;
	World()
	{
		ExternUpdate = ExternLoad = false;
		Ads = new AdList();
		Entities = new EntityList();
		SimulationAds = new AdList();
		SimulationEntities = new EntityList();
		Attributes = new AttributeList();
		nullatr = new EntityAttribute(true);
		ScoreNum = 0;
	};
	void AddEntity(std::shared_ptr<Entity> Entity_To_Add)
	{
		Entities->push_back(Entity_To_Add);
	};
	void AddEntity()
	{
		Entities->push_back(std::shared_ptr<Entity>(new Entity));
	};
	void AddAdvertisement(std::shared_ptr<Advertisement> Advertisement_To_Add)
	{
		Ads->push_back(Advertisement_To_Add);
	}
	void AddAdvertisement()
	{
		Ads->push_back(std::shared_ptr<Advertisement>(new Advertisement));
	};
	void AddSimulationEntity()
	{
		SimulationEntities->push_back(std::shared_ptr<Entity>(new Entity));
	};
	void AddSimulationAdvertisement()
	{
		SimulationAds->push_back(std::shared_ptr<Advertisement>(new Advertisement));
	};



	bool Update_No_LOD(int rad) //Updates all entities. Has no level of detail management (Our back propogation engine)
	{
		if (SimulationEntities->size() == 0)
			return true; //Everyone is dead
		for (auto it = SimulationEntities->begin(); it != SimulationEntities->end(); it++)
		{
			if (!it->get()->IsDead())
				int k = UpdateEntity(it - SimulationEntities->begin(), rad);
		}
		return false;
	}
	Entity *CopyEntityForSim(std::string Name, VECTOR2DF position, int i)
	{
		InheritsFromEntity(Name)->createCopy(SimulationEntities->at(i).get()); //Copy the entity from "Entities" to simulational entities
		int m = PrevInheritanceEntity(Name);
		SimulationEntities->at(i).get()->Link = Name;
		SimulationEntities->at(i).get()->Name = Name + ", Poly " + std::to_string(m);
		SimulationEntities->at(i).get()->LinkNumber = m;
		SimulationEntities->at(i).get()->Position = position;
		return SimulationEntities->at(SimulationEntities->size() - 1).get();
	}
	Advertisement *CopyAdForSim(std::string Name, VECTOR2DF position, int i)
	{
		InheritsFromAd(Name)->createCopy(SimulationAds->at(i).get()); //Copy the entity from "Ads" to simulational Ads
		int m = PrevInheritanceAd(Name);
		SimulationAds->at(i).get()->Link = Name;
		SimulationAds->at(i).get()->Name = Name + ", Poly " + std::to_string(m);
		SimulationAds->at(i).get()->LinkNumber = m;
		SimulationAds->at(i).get()->Position = position;
		return SimulationAds->at(SimulationAds->size() - 1).get();
	}
	Entity *ReturnEntity(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)Entities->size(); i++)
		if (Name == Entities->at(i).get()->Name)
			return Entities->at(i).get();
		return nullptr;
	};
	int EntityIndex(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)Entities->size(); i++)
		if (Name == Entities->at(i).get()->Name)
			return i;
		return -1;
	};
	Entity *ReturnSimulationalEntity(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)SimulationEntities->size(); i++)
		if (Name == SimulationEntities->at(i).get()->Name)
			return SimulationEntities->at(i).get();
		return nullptr;
	};
	int SimulationalEntityIndex(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)SimulationEntities->size(); i++)
		if (Name == SimulationEntities->at(i).get()->Name)
			return i;
		return -1;
	};
	Advertisement *ReturnAdvertisement(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)Ads->size(); i++)
		if (Name == Ads->at(i).get()->Name)
			return Ads->at(i).get();
		return nullptr;
	};
	int AdIndex(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)Ads->size(); i++)
		if (Name == Ads->at(i).get()->Name)
			return i;
		return -1;
	};
	Advertisement *ReturnSimulationalAdvertisement(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)SimulationAds->size(); i++)
		if (Name == SimulationAds->at(i).get()->Name)
			return SimulationAds->at(i).get();
		return nullptr;
	};
	int SimulationAdIndex(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)SimulationAds->size(); i++)
		if (Name == SimulationAds->at(i).get()->Name)
			return i;
		return -1;
	};

	int AttributeIndex(std::string Name)
	{
		for (int i = 0; i < (int)this->Attributes->size(); i++)
		if (Name == Attributes->at(i).Name)
			return i;
		return -1;
	}
	bool AttributeExist(std::string Name)
	{
		for (int i = 0; i < (int)Entities->size(); i++)
		{
			for (int j = 0; j < (int)Entities->at(i)->Attributes.size(); j++)
			{
				if (Entities->at(i)->Attributes.at(j)->Name == Name)
					return true;
			}
		}
		return false;
	}
	void ResizeEntityAttributes()
	{
		for (int i = 0; i < (int)Entities->size(); i++)
		{
			Entities->at(i).get()->UsedAttributes.push_back(-1);
		}
	}
	void BuildAdvertisement(Advertisement *ad)
	{
		for (int j = 0; j < (int)ad->negative_effects.size(); j++)
		{
			ad->negative_effects.at(j).Index = this->AttributeIndex(ad->negative_effects.at(j).StringT); //Make sure the ads are trying to effect an attribute that actually exists
		}
		for (int j = 0; j < (int)ad->positive_effects.size(); j++)
		{
			ad->positive_effects.at(j).Index = this->AttributeIndex(ad->positive_effects.at(j).StringT); //Make sure the ads are trying to effect an attribute that actually exists
		}
		if (ad->Infer != nullptr)
			BuildAdvertisement(ad->Infer);
	}
	void BuildAdvertisementLinks()
	{
		for (int i = 0; i < (int)Ads->size(); i++)
		{
			BuildAdvertisement(Ads->at(i).get());
		}
		for (int k = 0; k < Entities->size(); k++)
		{
			auto Ads2 = &Entities->at(k)->SelfAdvertisment;
			for (int i = 0; i < (int)Ads2->size(); i++)
			{
				BuildAdvertisement(Ads2->at(i).get());
			}

			auto Ads3 = &Entities->at(k)->BroadcastedAdvertisement;
			for (int i = 0; i < (int)Ads3->size(); i++)
			{
				BuildAdvertisement(Ads3->at(i).get());
			}
		}
	}
	void Build()
	{
		
		BuildAdvertisementLinks();
		for (int i = 0; i < (int)Entities->size(); i++) //For every entity
		{ 
			for (int j = 0; j < (int)Ads->size(); j++) //Cycle through every ad, and see if this entity is compadible with that ad
			{
				bool tempbool = true;
				for (int k = 0; k < (int)Ads->at(j).get()->positive_effects.size(); k++)
					//Search through all of their attributes, and see if there are any mismatches
					if (Entities->at(i).get()->UsedAttributes.at(Ads->at(j).get()->positive_effects.at(k).Index)== - 1) //The attribute was not found
					{
						tempbool = false; //The advertisement isn't compadible with this entity
						break;
					}
				if (tempbool)
				{
					for (int k = 0; k < (int)Ads->at(j).get()->negative_effects.size(); k++)
						if (Entities->at(i).get()->UsedAttributes.at(Ads->at(j).get()->negative_effects.at(k).Index) == -1) //The attribute was not found
						{
							tempbool = false;
							break;
						}
				}
				if (tempbool)
					Entities->at(i).get()->ViableAds.push_back(j); //There were no mismatches, therefore the ad is viable
			};
		}
		for (int i = 0; i < (int)Entities->size(); i++)
		{
			std::sort(Entities->at(i).get()->ViableAds.begin(), Entities->at(i).get()->ViableAds.end());
		}
		PrepareForSimulation();
	}
	void PrepareForSimulation()
	{
		for (int i = 0; i < (int)SimulationEntities->size(); i++)
		{
			ReturnEntity(SimulationEntities->at(i).get()->Link)->createUpdateCopy(SimulationEntities->at(i).get());

		}
		for (int i = 0; i < (int)SimulationAds->size(); i++)
		{
			Advertisement *ad = ReturnAdvertisement(SimulationAds->at(i).get()->Link);
			if (ad != nullptr)
				ad->createUpdateCopy(SimulationAds->at(i).get());
			else 
				SimulationAds->erase(SimulationAds->begin() + i);
			SimulationAds->at(i).get()->LinkAddress = AdIndex(SimulationAds->at(i).get()->Link);
		}
	}
	int UpdateEntity(int EntityIndex, int rad) //Just a generic update function. Returns 0 when the entity is busy, returns 1 when the entity has been given a new task
	{
		AdList A;
		std::vector<float> Scores;
		bool b = true;			bool q = false; 			int intf = 0;
		switch (SimulationEntities->at(EntityIndex).get()->ReturnLockVal())
		{
		case 1: //The lock is over
			SimulationEntities->at(EntityIndex).get()->ExecuteQueue();
			SimulationEntities->at(EntityIndex).get()->IncLock();
			return 0;
			break;
		case 0: //The lock ended last itt, give me a new task
			Scores = std::vector<float>();
			A = FindLocalAds(EntityIndex, rad);
			for (int i = 0; i < (int)SimulationEntities->at(EntityIndex).get()->SelfAdvertisment.size(); i++)
			{
				A.push_back(SimulationEntities->at(EntityIndex).get()->SelfAdvertisment.at(i));
			}
			for (int i = 0; i < (int)A.size();) //Either its not in use, or its in use, but sharable between people
			{
				if (A.at(i).get()->IsInUse() && !A.at(i).get()->UsableByMoreThanOne)
					A.erase(A.begin() + i); //So we erase it from the vector, and get the next highest
				else if (A.at(i).get()->OwnerSet)
				{
					if (A.at(i).get()->GetOwner() != SimulationEntities->at(EntityIndex).get())
					{
						A.erase(A.begin() + i);
					}
					else
						i++;
				}
				else
					i++;
			}
			for (int i = 0; i < (int)A.size();)
			{
				float ftemp = SimulationEntities->at(EntityIndex).get()->ReturnScore(*A.at(i).get());
				ScoreNum += (double)A.at(i).get()->positive_effects.size() + (double)A.at(i).get()->negative_effects.size();
				if (A.at(i).get()->Infer != nullptr)
					ScoreNum += (double)A.at(i).get()->Infer->positive_effects.size() + (double)A.at(i).get()->Infer->negative_effects.size();
				Scores.push_back(ftemp);
				i++;


			}
			
			int index;
			index = std::distance(Scores.begin(), std::max_element(Scores.begin(), Scores.end())); //Where does the highest value occur?
			if (!A.at(index)->UsableByMoreThanOne)
				A.at(index).get()->SetOwner(SimulationEntities->at(EntityIndex).get()); //If it isn't sharable, set the owner to ourself. We could eventually set this up as a job application idealogy 

			SimulationEntities->at(EntityIndex).get()->LockForTurns(A.at(index));
			A.at(index)->BeginUse();
			return 1;
		default: //incriment the itt
			SimulationEntities->at(EntityIndex).get()->Update();
			return 0;
			break;
		}
	};

	//CAN BE MULTITHREADED

	AdList FindLocalAds(int EntityIndex, int rad) //Looks to see how many advertisements are within walking distance of the entity
	{
		AdList TempList = AdList();
		Entity *t = SimulationEntities->at(EntityIndex).get();
		VECTOR2DF temp = t->Position;
		for (int i = 0; i < (int)SimulationAds->size(); i++)
		{
			if (!std::binary_search(t->ViableAds.begin(), t->ViableAds.end(), SimulationAds->at(i).get()->LinkNumber)) //This ad doesn't aplpy 
				i++;
			if (i >= (int)SimulationAds->size())
				break;
			if (rad > SimulationAds->at(i).get()->Position.Distance(temp)) //Simple bounding sphere
				TempList.push_back(SimulationAds->at(i));
		}
		return TempList;
	};

	//SHARED POINTER MANAGEMENT

	std::shared_ptr<Advertisement> ReturnAdvertisementShared(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)Ads->size(); i++)
			if (Name == Ads->at(i).get()->Name)
				return Ads->at(i);
		return nullptr;
	};
	std::shared_ptr<Advertisement> ReturnAdvertisementShared(std::string Name, Entity *et) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)et->SelfAdvertisment.size(); i++)
			if (Name == et->SelfAdvertisment.at(i).get()->Name)
				return et->SelfAdvertisment.at(i);
		for (int i = 0; i < (int)et->BroadcastedAdvertisement.size(); i++)
			if (Name == et->BroadcastedAdvertisement.at(i).get()->Name)
				return et->BroadcastedAdvertisement.at(i);

		return nullptr;
	};
	std::shared_ptr<Entity> ReturnEntityShared(std::string Name) //ONLY TO BE USED IN THE EDITOR!!
	{
		for (int i = 0; i < (int)Entities->size(); i++)
			if (Name == Entities->at(i).get()->Name)
				return Entities->at(i);
		return nullptr;
	};

	int DeleteEntityAdvertisement(std::string Name, Entity *et)
	{
		int ret = 0;
		for (int i = 0; i < (int)et->SelfAdvertisment.size(); i++){
			if (Name == et->SelfAdvertisment.at(i).get()->Name)
			{
				et->SelfAdvertisment.erase(et->SelfAdvertisment.begin() + i);
				ret = 1;
				break;
			}
		}
		for (int i = 0; i < (int)et->BroadcastedAdvertisement.size(); i++){
			if (Name == et->BroadcastedAdvertisement.at(i).get()->Name)
			{
				et->BroadcastedAdvertisement.erase(et->BroadcastedAdvertisement.begin() + i);
				ret = 1;
				break;
			}
		}
		return ret;
	}
private:

	Entity *InheritsFromEntity(std::string OtherEntity)
	{
		Entity *Ptr = ReturnEntity(OtherEntity);
		return Ptr;
	};
	Entity InheritsFrom_StaticEntity(std::string OtherEntity)
	{
		return ReturnEntity(OtherEntity)->createCopy();
	}
	Advertisement *InheritsFromAd(std::string OtherAd)
	{
		Advertisement *Ptr = ReturnAdvertisement(OtherAd);
		return Ptr;
	}
	Advertisement InheritsFrom_StaticAd(std::string OtherAd)
	{
		return ReturnAdvertisement(OtherAd)->createCopy();
	}

	int PrevInheritanceEntity(std::string Name)
	{
		int j = 0;
		for (int i = 0; i < (int)SimulationEntities->size(); i++)
			if (SimulationEntities->at(i).get()->Link == Name)
				j++;
		return j;
	}
	int PrevInheritanceAd(std::string Name)
	{
		int j = 0;
		for (int i = 0; i < (int)SimulationAds->size(); i++)
			if (SimulationAds->at(i).get()->Link == Name)
				j++;
		return j;
	}
};
#endif