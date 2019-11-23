#include "settings.h"
#include <string>
#include <sstream>
#include "Xorstr.hpp"
#include "Util.h"
std::vector<TeleporterInfo> Settings::TeleportHelper::TeleporterInfos = {};
bool Settings::TeleportHelper::enabled = false;
std::string Settings::TeleportHelper::actMapName;

void GetVal(Json::Value& config, int* setting)
{
	if (config.isNull())
		return;

	*setting = config.asInt();
}

void GetVal(Json::Value& config, bool* setting)
{
	if (config.isNull())
		return;

	*setting = config.asBool();
}

void GetVal(Json::Value& config, float* setting)
{
	if (config.isNull())
		return;

	*setting = config.asFloat();
}

void GetVal(Json::Value& config, ImColor* setting)
{
	if (config.isNull())
		return;

	GetVal(config["r"], &setting->Value.x);
	GetVal(config["g"], &setting->Value.y);
	GetVal(config["b"], &setting->Value.z);
	GetVal(config["a"], &setting->Value.w);
}

void GetVal(Json::Value& config, char** setting)
{
	if (config.isNull())
		return;

	*setting = strdup(config.asCString());
}

void GetVal(Json::Value& config, char* setting)
{
	if (config.isNull())
		return;

	strcpy(setting, config.asCString());
}

void GetVal(Json::Value& config, ColorVar* setting)
{
	if (config.isNull())
		return;

	GetVal(config["r"], &setting->color.Value.x);
	GetVal(config["g"], &setting->color.Value.y);
	GetVal(config["b"], &setting->color.Value.z);
	GetVal(config["a"], &setting->color.Value.w);
	GetVal(config["rainbow"], &setting->rainbow);
	GetVal(config["rainbowSpeed"], &setting->rainbowSpeed);
}

void GetVal(Json::Value& config, HealthColorVar* setting)
{
	if (config.isNull())
		return;

	GetVal(config["r"], &setting->color.Value.x);
	GetVal(config["g"], &setting->color.Value.y);
	GetVal(config["b"], &setting->color.Value.z);
	GetVal(config["a"], &setting->color.Value.w);
	GetVal(config["rainbow"], &setting->rainbow);
	GetVal(config["rainbowSpeed"], &setting->rainbowSpeed);
	GetVal(config["hp"], &setting->hp);
}

template <typename Ord, Ord(*lookupFunction)(std::string)>
void GetOrdinal(Json::Value& config, Ord* setting)
{
	if (config.isNull())
		return;

	Ord value;
	if (config.isString())
		value = lookupFunction(config.asString());
	else
		value = (Ord)config.asInt();

	*setting = value;
}

void LoadColor(Json::Value& config, ImColor color)
{
	config["r"] = color.Value.x;
	config["g"] = color.Value.y;
	config["b"] = color.Value.z;
	config["a"] = color.Value.w;
}

void LoadColor(Json::Value& config, ColorVar color)
{
	config["r"] = color.color.Value.x;
	config["g"] = color.color.Value.y;
	config["b"] = color.color.Value.z;
	config["a"] = color.color.Value.w;
	config["rainbow"] = color.rainbow;
	config["rainbowSpeed"] = color.rainbowSpeed;
}

void LoadColor(Json::Value& config, HealthColorVar color)
{
	config["r"] = color.color.Value.x;
	config["g"] = color.color.Value.y;
	config["b"] = color.color.Value.z;
	config["a"] = color.color.Value.w;
	config["rainbow"] = color.rainbow;
	config["rainbowSpeed"] = color.rainbowSpeed;
	config["hp"] = color.hp;
}

void Settings::LoadDefaultsOrSave(std::string path)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;

	LoadColor(settings[XorStr("UI")][XorStr("mainColor")], Settings::UI::mainColor);
	LoadColor(settings[XorStr("UI")][XorStr("bodyColor")], Settings::UI::bodyColor);
	LoadColor(settings[XorStr("UI")][XorStr("fontColor")], Settings::UI::fontColor);
	LoadColor(settings[XorStr("UI")][XorStr("accentColor")], Settings::UI::accentColor);

	std::ofstream(path) << styledWriter.write(settings);
}

inline bool exists_test3(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}
void Settings::LoadConfig(std::string path)
{
	if (!exists_test3(path))
	{
		//Settings::LoadDefaultsOrSave(path);
		return;
	}

	Json::Value settings;
	std::ifstream configDoc(path, std::ifstream::binary);
	configDoc >> settings;

	GetVal(settings[XorStr("UI")][XorStr("mainColor")], &Settings::UI::mainColor);
	GetVal(settings[XorStr("UI")][XorStr("bodyColor")], &Settings::UI::bodyColor);
	GetVal(settings[XorStr("UI")][XorStr("fontColor")], &Settings::UI::fontColor);
	GetVal(settings[XorStr("UI")][XorStr("accentColor")], &Settings::UI::accentColor);

}


char paths[255];
void Settings::LoadSettings()
{
	//static CHAR path[MAX_PATH];
	strcat(paths, "C:\\Xhook\\wow");
	std::string folder, file;

	folder = std::string(paths) + "\\Configs\\";

	CreateDirectoryA(folder.c_str(), NULL);
}


void Settings::SaveTeleporterInfo(std::string path) {
	Json::Value grenadeInfos;
	for (auto TeleporterInfo = TeleportHelper::TeleporterInfos.begin();
		TeleporterInfo != TeleportHelper::TeleporterInfos.end(); TeleporterInfo++) {
		Json::Value act;

		act[XorStr("pos")][XorStr("x")] = TeleporterInfo->pos.x;
		act[XorStr("pos")][XorStr("y")] = TeleporterInfo->pos.y;
		act[XorStr("pos")][XorStr("z")] = TeleporterInfo->pos.z;

		act[XorStr("name")] = TeleporterInfo->name.c_str();

		act[XorStr("currmsg")] = TeleporterInfo->currmsg;

		grenadeInfos.append(act);
	}

	Json::Value data;
	Json::StyledWriter styledWriter;

	data[XorStr("teleportinfo")] = grenadeInfos;

	std::ofstream(path) << styledWriter.write(data);
}

void Settings::LoadTeleporterInfo(std::string path) {
	/*if (!std::ifstream(path).good() || !DoesFileExist(path.c_str()))
		return;*/
	Json::Value data;
	std::ifstream configDoc(path, std::ifstream::binary);
	try {
		configDoc >> data;
	}
	catch (...) {
		//pCvar->ConsoleDPrintf(XorStr("Error parsing the config file.\n"));
		printf("Error parsing the config file.\n");
		return;
	}

	Json::Value array = data[XorStr("teleportinfo")];
	Settings::TeleportHelper::TeleporterInfos = {};

	for (Json::Value::iterator it = array.begin(); it != array.end(); ++it) {
		
		Json::Value act = *it;

		Json::Value pos = act[XorStr("pos")];
		Vector currpos = Vector(pos[XorStr("x")].asFloat(), pos[XorStr("y")].asFloat(),
			pos[XorStr("z")].asFloat());

		const char* name = act[XorStr("name")].asCString();

		Json::Value angle = act[XorStr("angle")];
		Vector vAngle = Vector(angle[XorStr("x")].asFloat(), angle[XorStr("y")].asFloat(), 0);

		int currmsg = act[XorStr("currmsg")].asInt();

		Settings::TeleportHelper::TeleporterInfos.push_back(TeleporterInfo(currpos, name, currmsg));
	}
}