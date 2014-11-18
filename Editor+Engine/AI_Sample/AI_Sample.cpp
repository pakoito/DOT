// AI_Sample.cpp : main project file.

#include "stdafx.h"
#define AILINK_EXPORTS
#include "AILink.h"
#include "World.h"
using namespace System;
#define NO_SOCIALIZATION //The feature is too experimental

int runner::run(World* currentworld, int *k, int *i, bool *running)
{

    std::clock_t start;
    double duration;

    start = std::clock();
    std::clock_t start2 = std::clock();
    *k = 0;
    while (!currentworld->Update_No_LOD(10) && *running)
    {
        (*i) += 1;
		cout << i << endl;
        if (*i % 10000 == 0)
        {
            (*k) += 1;
            duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
            double duration2 = (std::clock() - start2) / (double)CLOCKS_PER_SEC;
            int q = static_cast<int>((10000 / duration) * currentworld->Entities->size());
            std::cout << "" << q << " Entites per second, currently on itteration: " << (*k) << ", time: " << (int)duration2 << " seconds\n";
            //Console::ReadLine();
            start = std::clock();
        }
    }
    return 0;
}

int runner::startup(World* currentworld)//array<System::String ^> ^args)
{
	//std::shared_ptr<Advertisement> ad (new Advertisement);
	//std::shared_ptr<Advertisement> ad2 (new Advertisement);
	//std::shared_ptr<Advertisement> ad3 (new Advertisement);
	//std::shared_ptr<Advertisement> ad4(new Advertisement);
	//std::shared_ptr<Advertisement> ad5(new Advertisement);
	//std::shared_ptr<Advertisement> ad6(new Advertisement);
	//std::shared_ptr<Advertisement> ad7(new Advertisement);

	//ad.get()->Name = "Resturaunt";  //He lives near a restaurant
	//ad.get()->TimeConstant = 2; //It takes 2 turns for him to eat
	//ad.get()->Position = VECTOR2DF(0, 0); //And its located quite from from him
	//ad.get()->positive_effects.push_back(Cost("Food",40)); //It gives him +20 food
	//ad.get()->negative_effects.push_back(Cost("Wealth",10)); //It costs him 10 dollars

	//ad2.get()->Name = "Cuisine";
	//ad2.get()->TimeConstant = 2;
	//ad2.get()->Position = VECTOR2DF(0, 0);
	//ad2.get()->positive_effects.push_back(Cost("Food", 50)); //It gives him +50 food
	//ad2.get()->negative_effects.push_back(Cost("Wealth", 40));

	//ad3.get()->Name = "Farming"; //He's a farmer
	//ad3.get()->TimeConstant = 5; //It takes 5 turns to work
	//ad3.get()->Position = VECTOR2DF(0, 0); //And its located quite far from him
	//ad3.get()->positive_effects.push_back(Cost("Wealth", 20)); //It gives him +10 money
	//ad3.get()->positive_effects.push_back(Cost("Food", 5)); //Adds 4 to hunger
	//ad3.get()->negative_effects.push_back(Cost("Health", 1)); //This takes 1 from health (Its not a very taxing job apparently)
	//ad3.get()->UsableByMoreThanOne = false;

	//ad4.get()->Name = "Smith";
	//ad4.get()->TimeConstant = 7;
	//ad4.get()->Position = VECTOR2DF(0, 0);
	//ad4.get()->positive_effects.push_back(Cost("Wealth", 30)); //It gives him +50 money
	//ad4.get()->negative_effects.push_back(Cost("Health", 5)); //But it costs him 5 health (Technically with the 6 time constant, its 11 health)
	//ad4.get()->UsableByMoreThanOne = false;

	//ad5.get()->Name = "Second Smith";
	//ad5.get()->TimeConstant = 7;
	//ad5.get()->Position = VECTOR2DF(0, 0);
	//ad5.get()->positive_effects.push_back(Cost("Wealth", 30)); //It gives him +100 money
	//ad5.get()->negative_effects.push_back(Cost("Health", 5)); //But it costs him 50 health 
	//ad5.get()->UsableByMoreThanOne = false;

	//ad6.get()->Name = "Doctor";
	//ad6.get()->TimeConstant = 1; //It takes 1 turn for him to get better
	//ad6.get()->Position = VECTOR2DF(0, 0); //And its close to him
	//ad6.get()->positive_effects.push_back(Cost("Health", 30)); //It gives him +40 health
	//ad6.get()->negative_effects.push_back(Cost("Wealth", 10)); //It costs him 15 dollars


 //   currentworld->AddEntity();
 //   currentworld->AddEntity();
 //   currentworld->AddEntity();
 //   for (int i = 0; i < 3; ++i)
 //   {
 //       //currentworld->Entities->at(i).get()->Position = new VECTOR2DF(0, 0);
	//	currentworld->Entities->at(i).get()->Name = std::string("Entity " + std::to_string(i));
 //       std::map<std::string,EntityAttribute>* ats = currentworld->Entities->at(i).get()->getGoalsPtr();
 //       //(*ats)[0].Value = 0; //set health to 0
	//	//if (i == 0)
	//	//{
	//	//	(*ats)[0].Value = 90; //set health low
	//	//	(*ats)[0].grad = Gradient(20, 40, 60, 90,100); //Modify the gradient so it demands more food
	//	//}
 //   }
	//System::String ^Temp;
	//Temp = "";

	//currentworld->AddAdvertisement(ad);
	//currentworld->AddAdvertisement(ad2);
	//currentworld->AddAdvertisement(ad3); 
	//currentworld->AddAdvertisement(ad4);
	//currentworld->AddAdvertisement(ad5);
	//currentworld->AddAdvertisement(ad6);
	//currentworld->AddAdvertisement(ad7);
#ifndef NO_SOCIALIZATION //These entities are now chatable. THIS FEATURE HAS BEEN MOVED TO DIAG. DO NOT USE IT!!!
	currentworld->AddAdvertisement(Entity);
	currentworld->AddAdvertisement(Entity2);
#endif

    return 0;
}
