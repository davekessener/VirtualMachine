#include <iostream>
#include <unistd.h>
//#include "surface/PNGLoader.h"
//#include "surface/GLImageBuffer.h"
//#include "surface/Manager.h"
//#include "surface/Label.h"
//#include "surface/Icon.h"
//#include "surface/Button.h"
#include <aux>
#include "Species.h"
#include "Move.h"
#include "Pokemon.h"
#include "UUID.hpp"

nbt::TAG_List_ptr_t generateSpecies( );
nbt::TAG_List_ptr_t generateMoves( );

void printSpecies(std::ostream&, const pkmn::Species&);

void run(void)
{
	nbt::TAG_Compound_ptr_t root = nbt::Make<nbt::TAG_Compound>();
	root->setTagList("Species", generateSpecies());
	root->setTagList("Moves", generateMoves());

	pkmn::SpeciesManager::Load(root->getTagList("Species"));
	pkmn::Moves::Load(root->getTagList("Moves"));

	printSpecies(std::cout, pkmn::SpeciesManager::Get("charizard"));

	nbt::writeFile("pokemon.nbt", root);
}

void printSpecies(std::ostream& os, const pkmn::Species& sp)
{
	os	<< "ID : '" << sp.id << "'\n"
		<< "Name : '" << sp.name << "'\n"
		<< "Types : ['" << sp.types[0] << "', '" << sp.types[1] << "']\n"
		<< "Abilities : ['" << sp.abilities[0] << "', '" << sp.abilities[1] << "']\n"
		<< "Hidden Ability : '" << sp.hidden_ability << "'\n"
		<< "Gender Ratio : '" << sp.gender_ratio << "'\n"
		<< "Is Genderless : '" << (sp.is_genderless ? "TRUE" : "FALSE") << "'\n"
		<< "Is Baby : '" << (sp.is_baby ? "TRUE" : "FALSE") << "'\n"
		<< "Catch Rate : '" << sp.catch_rate << "'\n"
		<< "Egg Groups : ['" << sp.egg_groups[0] << "', '" << sp.egg_groups[1] << "']\n"
		<< "Egg Cycles : '" << sp.egg_cycles << "'\n"
		<< "Base Experience : '" << sp.base_exp << "'\n"
		<< "Base Happiness : '" << sp.base_happiness << "'\n"
		<< "Growth Rate : '" << sp.growth_rate << "'\n"
		<< "Stats : ['" << sp.stats[0] << "'";

	for(int i = 1 ; i < 6 ; ++i)
	{
		os << ", '" << sp.stats[i] << "'";
	}

	os	<< "]\n"
		<< "EVs : ['" << sp.evs[0] << "'";

	for(int i = 1 ; i < 6 ; ++i)
	{
		os << ", '" << sp.evs[i] << "'";
	}

	os	<< "]\n";

	os	<< "TMs : [";
	bool f = true;
	for(uint tm : sp.tms)
	{
		if(!f) os << ", ";
		os << "'" << tm << "'";
		f = false;
	}
	os << "]\n";

	os	<< "HMs : [";
	f = true;
	for(uint hm : sp.hms)
	{
		if(!f) os << ", ";
		os << "'" << hm << "'";
		f = false;
	}
	os << "]\n";

	os << "'Moves' :\n[";
	f = true;
	for(auto& m : sp.moves)
	{
		if(!f) os << ",";
		os << "\n\t{ID : '" << m.id << "', Level : '" << m.lvl << "'}";
		f = false;
	}
	os << "\n]\n";

	os	<< "Dex : { Number : '" << sp.dex.number 
		<< "', Height : '" << sp.dex.height 
		<< "', Weight : '" << sp.dex.weight 
		<< "', Color : '" << sp.dex.color << "' }\n";
}

nbt::TAG_Compound_ptr_t generateChrizard(void)
{
	nbt::TAG_Compound_ptr_t tag = nbt::Make<nbt::TAG_Compound>();

	tag->setString("ID", "charizard");
	tag->setString("Name", "Charizard");
	nbt::TAG_List_ptr_t types = nbt::Make<nbt::TAG_List>();
	types->addTag(nbt::Make<nbt::TAG_String>("", "fire"));
	types->addTag(nbt::Make<nbt::TAG_String>("", "flying"));
	tag->setTagList("Types", types);
	nbt::TAG_List_ptr_t abilities = nbt::Make<nbt::TAG_List>();
	abilities->addTag(nbt::Make<nbt::TAG_String>("", "blaze"));
	tag->setTagList("Abilities", abilities);
	tag->setString("HiddenAbility", "solar-power");
	tag->setFloat("GenderRatio", 0.875);
	tag->setByte("IsGenderless", 0);
	tag->setByte("IsBaby", 0);
	tag->setFloat("CatchRate", 5.9);
	tag->setInt("EggCycles", 21);
	tag->setInt("BaseExperience", 209);
	tag->setInt("BaseHappiness", 70);
	tag->setInt("GrowthRate", 4);
	nbt::TAG_List_ptr_t egg_groups = nbt::Make<nbt::TAG_List>();
	egg_groups->addTag(nbt::Make<nbt::TAG_String>("", "Dragon"));
	egg_groups->addTag(nbt::Make<nbt::TAG_String>("", "Monster"));
	tag->setTagList("EggGroups", egg_groups);
	tag->setByteArray("Stats", std::vector<BYTE>{78, 84, 78, 109, 85, 100});
	tag->setByteArray("EVs", std::vector<BYTE>{0, 0, 0, 3, 0, 0});
	tag->setByteArray("TMs", std::vector<BYTE>());
	tag->setByteArray("HMs", std::vector<BYTE>());
	nbt::TAG_List_ptr_t moves = nbt::Make<nbt::TAG_List>();
	nbt::TAG_Compound_ptr_t tackle_0 = nbt::Make<nbt::TAG_Compound>();
	tackle_0->setString("ID", "tackle");
	tackle_0->setInt("Level", 1);
	moves->addTag(tackle_0);
	tag->setTagList("Moves", moves);
	nbt::TAG_Compound_ptr_t dex = nbt::Make<nbt::TAG_Compound>();
	dex->setInt("Number", 6);
	dex->setInt("Height", 170);
	dex->setInt("Weight", 905);
	dex->setString("Color", "red");
	tag->setCompoundTag("Dex", dex);

	return tag;
}

nbt::TAG_Compound_ptr_t generateTackle(void)
{
	nbt::TAG_Compound_ptr_t tag = nbt::Make<nbt::TAG_Compound>();

	tag->setString("ID", "tackle");
	tag->setString("Name", "Tackle");
	tag->setString("Description", "");
	tag->setString("Type", "normal");
	tag->setString("Category", "Physical");
	tag->setInt("Power", 50);
	tag->setFloat("Accuracy", 1.0);
	tag->setInt("PP", 35);
	tag->setInt("Priority", 0);
	tag->setString("Target", "Opponent");
	tag->setByte("MakesContact", 1);
	tag->setTagList("StatChanges", nbt::Make<nbt::TAG_List>("", nbt::TAG_Compound::ID));
	tag->setTagList("StatusChanges", nbt::Make<nbt::TAG_List>("", nbt::TAG_Compound::ID));

	return tag;
}

nbt::TAG_List_ptr_t generateSpecies(void)
{
	nbt::TAG_List_ptr_t species = nbt::Make<nbt::TAG_List>("Species");

	species->addTag(generateChrizard());

	return species;
}

nbt::TAG_List_ptr_t generateMoves(void)
{
	nbt::TAG_List_ptr_t moves = nbt::Make<nbt::TAG_List>("Moves");

	moves->addTag(generateTackle());

	return moves;
}

//namespace dav { namespace pkmn {
//
//namespace screen
//{
//	class DexAnimation : public Surface
//	{
//		public:
//			DexAnimation(const std::string& base) : base_(base), time_(0), cur_(0) { }
//		private:
//			void i_doInit( )
//			{
//				const int MAXFRAME = 47;
//				
//				frames_.resize(MAXFRAME);
//
//				for(int i = 0 ; i < MAXFRAME ; ++i)
//				{
//					frames_[i] = PNGLoader::LoadPNG(aux::stringify(base_, '-', i, ".png"));
//					GLImageBuffer::SetLinear(frames_[i].id);
//				}
//			}
//			void i_doUpdate(uint d)
//			{
//				const int FRAME = 1500 / 47;
//
//				if((time_ += d) >= FRAME)
//				{
//					cur_ = (cur_ + 1) % frames_.size();
//					time_ -= FRAME;
//					dirty(true);
//				}
//			}
//			void i_doRender( )
//			{
//				auto frame(frames_.at(cur_));
//				int w(width()), h(height());
//				
//				float f1(frame.u2 / frame.v2), f2(w / (float)h);
//				
//				if(f1 < f2)
//				{
//					w = f1 * h;
//				}
//				else if(f2 < f1)
//				{
//					h = w / f1;
//				}
//
//				draw(frame.id, 0, 0, frame.u2, frame.v2, 0, 0, w, h);
//			}
//		private:
//			std::string base_;
//			std::vector<text_info> frames_;
//			uint time_, cur_;
//	};
//}
//
//using screen::Surface_ptr;
//
//class Test : public screen::Surface
//{
//	using screen::Surface::MouseButtons;
//	public:
//	private:
//		void i_doInit( )
//		{
//			p_.x = p_.y = last_.x = last_.y = 0;
//		}
//		void i_doRender( )
//		{
//			auto s(getSize());
//			fillRect(0, 0, s.w, s.h, 0);
//			fillRect(1, 1, s.w - 1, s.h - 1, 0xffffff);
//			fillRect(p_.x - 5, p_.y - 5, p_.x + 5, p_.y + 5, 0xff0000);
//		}
//		void i_doMouseHover(int x, int y)
//		{
//			if(x != last_.x || y != last_.y)
//			{
//				p_.x = x + (x - last_.x);
//				p_.y = y + (y - last_.y);
//				last_.x = x; last_.y = y;
//				dirty(true);
//			}
//		}
//		coords p_, last_;
//};
//
//class Visualizer : public screen::Label
//{
//	private:
//		void i_doRender(void)
//		{
//			int w(width()), h(height());
//			fillRect(0, 0, w, h, 0x000000);
//			fillRect(1, 1, w - 1, h - 1, 0xffffff);
//			screen::Label::i_doRender();
//		}
//};
//
//class Root : public screen::Surface
//{
//	public:
//	private:
//		void i_doInit( )
//		{
//			addChild(test_ = std::make_shared<Test>());
//			test_->init(256, 256, 500, 500);
//
//			addChild(lbl_ = std::make_shared<Visualizer>(), 20);
//			lbl_->init(8, 8, 80, 80);
//			lbl_->message("This is a test!");
//			lbl_->center(true);
//			lbl_->spacing(lbl_->charsize());
//			
//			addChild(icon_ = std::make_shared<screen::Icon>(), 10);
//			icon_->init(160, 160, 16, 16);
//			icon_->load("icons/save.png");
//
//			addChild(button_ = std::make_shared<screen::Button>());
//			button_->init(100, 8, 64, 32);
//			button_->loadIcon("icons/split.png");
//			button_->loadText("OK");
//			
//			addChild(dex_ = std::make_shared<screen::DexAnimation>("dexsprites/006"));
//			dex_->init(8, 160, 133, 140);
//			
//			id = screen::PNGLoader::LoadSquarePNG("test.png");
//		}
//		void i_doRender( )
//		{
//			auto s(getSize());
//			fillRect(0, 0, s.w, s.h, 0xffffff);
//			draw(id, 0, 0, 1, 1, 800, 100, 832, 132);
//		}
//	private:
//		DWORD id;
//		Surface_ptr test_, dex_;
//		screen::Label_ptr lbl_;
//		screen::Icon_ptr icon_;
//		screen::Button_ptr button_;
//};
//
//}}

extern void run_test();

int main(int argc, char *argv[])
try
{
	chdir("resource/");

	run_test();
//	runProgram("Pokemon", std::make_shared<Root>());
//	run();

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

