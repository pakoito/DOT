#ifndef WXML_H
#define WXML_H
#include "runner.h"
#include <qxmlstream.h>
class XMLHandler
{
public:
	World *worldptr;
	GridScene *ptr;
	XMLHandler(QString Location, World *ptr, GridScene *grid = NULL, bool Saving = true) //If saving is false, then it loads a file
	{
		worldptr = ptr;
		this->ptr = grid;
		if (Saving)
			WriteTo(Location);
		else
			ReadFrom(Location);
	}
	void ReadFrom(QString Location)
	{
		worldptr->Entities->clear();
		worldptr->Ads->clear();
		worldptr->SimulationAds->clear();
		worldptr->SimulationEntities->clear();
		worldptr->Attributes->clear();
		ptr->Entities.clear();
		ptr->Ads.clear();
	

		QXmlStreamReader *Rxml = new QXmlStreamReader();

		QFile file(Location);
		if (!file.open(QFile::ReadOnly | QFile::Text))
		{
			std::cerr << "Error: Cannot read file " << qPrintable(Location)
				<< ": " << qPrintable(file.errorString())
				<< std::endl;

		}

		Rxml->setDevice(&file);
		Rxml->readNext();
		int EntityIndex = 0;
		int AdIndex = 0;
		int ver = 0;
		while (!Rxml->atEnd())
		{
			QXmlStreamReader::TokenType token = Rxml->readNext();
			if (Rxml->name() == "DOT_UNCOMPILED_V03")
			{
				ver = 3;
			}
			else if (Rxml->name() == "DOT_UNCOMPILED_V02")
			{
				ver = 2;
			}
			//If token is just StartDocument - go to next
			if (token == QXmlStreamReader::StartDocument) {

				continue;
			}
			//If token is StartElement - read it
			if (token == QXmlStreamReader::StartElement) {

				if (Rxml->name() == "Entity") {
					worldptr->AddEntity();
					Rxml->readNext();
					Rxml->readNext();
					worldptr->Entities->at(worldptr->Entities->size() - 1).get()->Name = Rxml->readElementText().toStdString();
					Entity *t = worldptr->Entities->at(worldptr->Entities->size() - 1).get();
					LoadEntity(t, Rxml, ver);
					t->createCopy(worldptr->Entities->at(worldptr->Entities->size() - 1).get());
					Rxml->readNext();
				}

				if (Rxml->name() == "Advertisement") {
					Rxml->readNext();
					worldptr->AddAdvertisement();

					LoadAdvertisement(worldptr->Ads->at(worldptr->Ads->size() - 1).get(), Rxml);
				}
				if (Rxml->name() == "SimulationalEntity") {
					worldptr->AddSimulationEntity();
					Rxml->readNext();
					Rxml->readNext();
					std::string Link = Rxml->readElementText().toStdString();
					Rxml->readNext();
					Rxml->readNext();
					float X = Rxml->readElementText().toFloat();
					Rxml->readNext();
					Rxml->readNext();
					float Y = Rxml->readElementText().toFloat();

					worldptr->CopyEntityForSim(Link, VECTOR2DF(X, Y), worldptr->SimulationEntities->size() - 1);
					EntityEdit Ade;
					Ade.index = (worldptr->SimulationEntities->size() - 1);
					Ade.Link = Link;
					Ade.rect = new QRect(X, Y, 50, 50);
					Ade.Position = VECTOR2DF(X, Y);
					Ade.Name = worldptr->SimulationEntities->at(worldptr->SimulationEntities->size() - 1).get()->Name;
					ptr->Entities.push_back(Ade);
				}
				if (Rxml->name() == "SimulationalAd") {
					worldptr->AddSimulationAdvertisement();
					Rxml->readNext();
					Rxml->readNext();
					std::string Link = Rxml->readElementText().toStdString();
					Rxml->readNext();
					Rxml->readNext();
					float X = Rxml->readElementText().toFloat();
					Rxml->readNext();
					Rxml->readNext();
					float Y = Rxml->readElementText().toFloat();

					worldptr->CopyAdForSim(Link, VECTOR2DF(X, Y), worldptr->SimulationAds->size() - 1);
					AdEdit Ade;
					Ade.index = (worldptr->SimulationAds->size() - 1);
					Ade.Link = Link;
					Ade.rect = new QRect(X, Y, 50, 50);
					Ade.Position = VECTOR2DF(X, Y);
					Ade.Name = worldptr->SimulationAds->at(worldptr->SimulationAds->size() - 1).get()->Name;
					ptr->Ads.push_back(Ade);
				}
			}
		}
		file.close();
	}
	void LoadEntity(Entity *et, QXmlStreamReader *Rxml, int ver = 2)
	{
		Rxml->readNext();
		QXmlStreamReader::TokenType token = Rxml->readNext();
		int t = Rxml->readElementText().toInt();
		int tempindex;
		bool Q = false;
		QStringRef m;
		int i = 0;
		std::string AttributeName;
		int Size = worldptr->Attributes->size();
		std::vector<EntityAttribute*> TempVector;
		if (Size != 0)
		{
			TempVector = std::vector<EntityAttribute*>(Size);
			et->UsedAttributes.resize(Size);
		}
		else
			TempVector = std::vector<EntityAttribute*>();
		int Val, Low, Q1, Q3, Max, Delta;

		while (i < t)
		{
			Rxml->readNext();
			Rxml->readNext();
			Rxml->readNext();
			Rxml->readNext();
			AttributeName = Rxml->readElementText().toStdString();
			Rxml->readNext();
			Rxml->readNext();
			Val = Rxml->readElementText().toInt();
			Rxml->readNext();
			Rxml->readNext();
			Delta = Rxml->readElementText().toInt();
			Rxml->readNext();
			Rxml->readNext();
			Low = Rxml->readElementText().toInt();
			Rxml->readNext();
			Rxml->readNext();
			Q1 = Rxml->readElementText().toInt();
			Rxml->readNext();
			Rxml->readNext();
			Q3 = Rxml->readElementText().toInt();
			Rxml->readNext();
			Rxml->readNext();
			Max = Rxml->readElementText().toInt();
			for (int j = 0; j < this->worldptr->Attributes->size(); j++)
			{
				if (worldptr->Attributes->at(j).Name == AttributeName) //We have seen it before
				{
					Q = true;
					tempindex = j;
					worldptr->Attributes->at(j).Amount++;
					break;
				}
			}
			if (Q == false) //This attribute hasn't been encountered before
			{
				tempindex = worldptr->Attributes->size();
				worldptr->Attributes->push_back(AttributeRef(AttributeName));
				worldptr->ResizeEntityAttributes();
				TempVector.resize(worldptr->Attributes->size());
			}
			et->UsedAttributes.at(tempindex) = worldptr->AttributeIndex(AttributeName); //Index in usedattributes = global address. Value = local
			TempVector.at(tempindex) = new EntityAttribute(AttributeName, Val, Delta, Gradient(Low, Q1, Q3, Max));
			et->Attributes.push_back(TempVector.at(tempindex));
			Rxml->readNext();
			Rxml->readNext();
			i++;
			Q = false;
		}

		Rxml->readNext();
		Rxml->readNext();
		t = Rxml->readElementText().toInt();
		Rxml->readNext();
		Rxml->readNext();
		i = 0;
		while (i < t)
		{
			et->SelfAdvertisment.push_back(std::shared_ptr<Advertisement>(new Advertisement));
			Rxml->readNext();
			LoadAdvertisement(et->SelfAdvertisment.at(et->SelfAdvertisment.size() - 1).get(), Rxml);
			Rxml->readNext();
			Rxml->readNext();
			i++;
		}
		t = Rxml->readElementText().toInt();
		Rxml->readNext();
		Rxml->readNext();
		i = 0;
		while (i < t)
		{
			et->BroadcastedAdvertisement.push_back(std::shared_ptr<Advertisement>(new Advertisement));
			Rxml->readNext();
			LoadAdvertisement(et->BroadcastedAdvertisement.at(et->BroadcastedAdvertisement.size() - 1).get(), Rxml);
			Rxml->readNext();
			Rxml->readNext();
			i++;
		}




	}
	int LoadAdvertisement(Advertisement *at, QXmlStreamReader *Rxml, bool isinf = false)
	{
		if (!isinf)
			Rxml->readNext();
		std::string AdvertisementName = Rxml->readElementText().toStdString();

		Rxml->readNext();
		Rxml->readNext();
		std::string Poly = Rxml->readElementText().toStdString();

		bool PolyB = true;
		if (Poly == "0")
			PolyB = false;

		Rxml->readNext();
		Rxml->readNext();
		int OwnerSet = Rxml->readElementText().toInt();

		bool OwnerSetB = true;
		if (OwnerSet == 0)
			OwnerSetB = false;
		Rxml->readNext();
		Rxml->readNext();
		int Time = Rxml->readElementText().toInt();

		at->Name = AdvertisementName;
		at->UsableByMoreThanOne = PolyB;
		at->OwnerSet = OwnerSetB;
		at->TimeConstant = Time;
		QXmlStreamReader::TokenType token;
		int i = 0;
		while (token != QXmlStreamReader::EndElement)
		{
			token = Rxml->readNext();
			if (token == QXmlStreamReader::TokenType::EndElement)
				break;
			token = Rxml->readNext();
			if ((token == QXmlStreamReader::TokenType::EndElement) && (Rxml->name() == "Inference")) //We're done with inferences
			{
				return -1;
			}
			if (Rxml->name() == "Name")
				return 2;
			if (Rxml->name() == "Inference")
			{

				if (token != QXmlStreamReader::EndElement)
				{
					QXmlStreamReader::TokenType token2;
					int tmp = 0;
					while (token2 != QXmlStreamReader::TokenType::EndElement)
					{
						if (tmp != 2)
							token2 = Rxml->readNext();

						if (token2 == QXmlStreamReader::TokenType::EndElement)
							break;

						at->Infer = new Advertisement();

						if (tmp != 2)
							tmp = LoadAdvertisement(at->Infer, Rxml, false); //No more inferences
						if (tmp == 2)
							tmp = LoadAdvertisement(at->Infer, Rxml, true); //Another inference
						i++;
						if (tmp != -1)
						{
							token2 = Rxml->readNext();
							if (token2 == QXmlStreamReader::TokenType::EndElement)
								break;
							token2 = Rxml->readNext();
							if (token2 == QXmlStreamReader::TokenType::EndElement)
								break;
						}
						else
							break;
					}
				}
			}
			if (Rxml->name() == "PositiveEffects")
			{
				QXmlStreamReader::TokenType token2 = QXmlStreamReader::TokenType::StartElement;
				while (token2 != QXmlStreamReader::TokenType::EndElement)
				{
					token2 = Rxml->readNext();
					if (token2 == QXmlStreamReader::TokenType::EndElement)
						break;
					token2 = Rxml->readNext();
					if (token2 == QXmlStreamReader::TokenType::EndElement)
						break;
					std::string Name = Rxml->readElementText().toStdString();
					token2 = Rxml->readNext();
					if (token2 == QXmlStreamReader::TokenType::EndElement)
						break;
					token2 = Rxml->readNext();
					if (token2 == QXmlStreamReader::TokenType::EndElement)
						break;
					int Val = Rxml->readElementText().toInt();
					Cost tempcost = Cost(Name, Val);
					tempcost.Index = 0;
					at->positive_effects.push_back(tempcost);
				}

			}
			if (Rxml->name() == "NegativeEffects") //There are no positive effects
			{
				QXmlStreamReader::TokenType token2;
				while (token2 != QXmlStreamReader::EndElement)
				{
					token2 = Rxml->readNext();
					if (token2 == QXmlStreamReader::TokenType::EndElement)
						break;
					token2 = Rxml->readNext();
					if (token2 == QXmlStreamReader::TokenType::EndElement)
						break;
					std::string Name = Rxml->readElementText().toStdString();
					token2 = Rxml->readNext();
					if (token2 == QXmlStreamReader::TokenType::EndElement)
						break;
					token2 = Rxml->readNext();
					if (token2 == QXmlStreamReader::TokenType::EndElement)
						break;
					int Val = Rxml->readElementText().toInt();
					at->negative_effects.push_back(Cost(Name, Val));
				}
				return 0;
			}
		}
		return 0;
	};
	void WriteTo(QString Location)
	{

		QFile file(Location);
		file.open(QIODevice::WriteOnly);

		QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(&file);
		xmlWriter->setAutoFormatting(true);
		xmlWriter->writeStartDocument();
		xmlWriter->writeStartElement("DOT_UNCOMPILED_V02");
		for (int i = 0; i < (int)worldptr->Entities->size(); i++)
		{
			xmlWriter->writeStartElement("Entity");
			WriteEntity(xmlWriter, worldptr, worldptr->Entities->at(i).get());
			xmlWriter->writeEndElement();
		}

		for (int i = 0; i < (int)worldptr->Ads->size(); i++)
		{
			xmlWriter->writeStartElement("Advertisement");
			WriteAdvertisement(xmlWriter, worldptr, worldptr->Ads->at(i).get());
			xmlWriter->writeEndElement();
		}

		if (worldptr->SimulationEntities->size() > 0)
		{

			for (int i = 0; i < (int)worldptr->SimulationEntities->size(); i++)
			{
				xmlWriter->writeStartElement("SimulationalEntity");
				xmlWriter->writeTextElement("Link", QString::fromStdString(worldptr->SimulationEntities->at(i).get()->Link));
				xmlWriter->writeTextElement("X", std::to_string(worldptr->SimulationEntities->at(i).get()->Position.Get_X()).c_str());
				xmlWriter->writeTextElement("Y", std::to_string(worldptr->SimulationEntities->at(i).get()->Position.Get_Y()).c_str());
				xmlWriter->writeEndElement();
			}
		}
		if (worldptr->SimulationAds->size() > 0)
		{

			for (int i = 0; i < (int)worldptr->SimulationAds->size(); i++)
			{
				xmlWriter->writeStartElement("SimulationalAd");
				xmlWriter->writeTextElement("Link", QString::fromStdString(worldptr->SimulationAds->at(i).get()->Link));
				xmlWriter->writeTextElement("X", QString::fromStdString(std::to_string(worldptr->SimulationAds->at(i).get()->Position.Get_X())));
				xmlWriter->writeTextElement("Y", QString::fromStdString(std::to_string(worldptr->SimulationAds->at(i).get()->Position.Get_Y())));
				xmlWriter->writeEndElement();
			}
		}
		xmlWriter->writeEndElement();
		xmlWriter->writeEndDocument();
		file.close();
	}
	void WriteEntity(QXmlStreamWriter *xmlWriter, World* worldptr, Entity *et)
	{

		xmlWriter->writeTextElement("Name", QString::fromStdString(et->Name));

		xmlWriter->writeTextElement("Amt", QString::fromStdString(std::to_string(et->Attributes.size())));
		for (int i = 0; i < et->Attributes.size(); i++)
		{
			WriteAttribute(xmlWriter, et->Attributes[i]);
		}
		xmlWriter->writeTextElement("Amt", QString::fromStdString(std::to_string(et->SelfAdvertisment.size())));
		xmlWriter->writeStartElement("SelfAdvertisements");
		for (int i = 0; i < et->SelfAdvertisment.size(); i++)
		{
			WriteAdvertisement(xmlWriter, worldptr, et->SelfAdvertisment.at(i).get());
		}
		xmlWriter->writeEndElement();
		xmlWriter->writeTextElement("Amt", QString::fromStdString(std::to_string(et->BroadcastedAdvertisement.size())));
		xmlWriter->writeStartElement("BroadcastedAdvertisements");
		for (int i = 0; i < et->BroadcastedAdvertisement.size(); i++)
		{
			WriteAdvertisement(xmlWriter, worldptr, et->BroadcastedAdvertisement.at(i).get());
		}
		xmlWriter->writeEndElement();
		xmlWriter->writeTextElement("Power", QString::fromStdString(std::to_string(et->Power)));
	}
	void WriteAttribute(QXmlStreamWriter *xmlWriter, EntityAttribute *it)
	{
		xmlWriter->writeStartElement("Attribute");
		xmlWriter->writeTextElement("Name", QString::fromStdString(it->Name));
		xmlWriter->writeTextElement("Value", QString::fromStdString(std::to_string(it->Value)));
		xmlWriter->writeTextElement("Delta", QString::fromStdString(std::to_string(it->Delta)));
		xmlWriter->writeTextElement("Low", QString::fromStdString(std::to_string(it->grad.Low)));
		xmlWriter->writeTextElement("Q1", QString::fromStdString(std::to_string(it->grad.Q1)));
		xmlWriter->writeTextElement("Q3", QString::fromStdString(std::to_string(it->grad.Q3)));
		xmlWriter->writeTextElement("Max", QString::fromStdString(std::to_string(it->grad.Max)));
		xmlWriter->writeEndElement();
	};
	void WriteAdvertisement(QXmlStreamWriter *xmlWriter, World *worldptr, Advertisement *ad)
	{
		xmlWriter->writeTextElement("Name", QString::fromStdString(ad->Name));
		xmlWriter->writeTextElement("Polyusability", QString::fromStdString(std::to_string(ad->UsableByMoreThanOne)));
		xmlWriter->writeTextElement("OwnerSet", QString::fromStdString(std::to_string(ad->OwnerSet)));
		xmlWriter->writeTextElement("TimeConstant", QString::fromStdString(std::to_string(ad->TimeConstant)));
		if (ad->OwnerSet == true) //Save who my owner is
		{
			xmlWriter->writeStartElement("Owner");
			int t = worldptr->EntityIndex(ad->GetOwner()->Name);
			if (t == -1) //It is bound to a simulation entity
			{
				t = worldptr->SimulationalEntityIndex(ad->GetOwner()->Name);
				xmlWriter->writeTextElement("SimulationOwner", QString::fromStdString(std::to_string(t)));
			}
			xmlWriter->writeTextElement("MasterOwner", QString::fromStdString(std::to_string(t)));
			xmlWriter->writeEndElement();
		}
		if (ad->Infer != nullptr) //Save our inferences
		{
			xmlWriter->writeStartElement("Inference");
			WriteAdvertisement(xmlWriter, worldptr, ad->Infer);
			xmlWriter->writeEndElement();
		}
		if (ad->positive_effects.size() > 0)
		{
			xmlWriter->writeStartElement("PositiveEffects");
			for (int i = 0; i < ad->positive_effects.size(); i++)
			{
				xmlWriter->writeTextElement("Attribute", QString::fromStdString(ad->positive_effects.at(i).StringT));
				xmlWriter->writeTextElement("Delta", QString::fromStdString(std::to_string(ad->positive_effects.at(i).DiminishingReturn)));
			}
			xmlWriter->writeEndElement();
		}
		if (ad->negative_effects.size() > 0)
		{
			xmlWriter->writeStartElement("NegativeEffects");
			for (int i = 0; i < ad->negative_effects.size(); i++)
			{
				xmlWriter->writeTextElement("Attribute", QString::fromStdString(ad->negative_effects.at(i).StringT));
				xmlWriter->writeTextElement("Delta", QString::fromStdString(std::to_string(ad->negative_effects.at(i).DiminishingReturn)));
			}
			xmlWriter->writeEndElement();
		}

	};
};
#endif