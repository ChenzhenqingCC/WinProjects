struct RoleInitialAbility {
	bool parse(XMLParser& xml, XMLParser::NodePtr node);
	bool reset();
	
	struct Init {
		bool parse(XMLParser& xml, XMLParser::NodePtr node);
		bool reset();
		
		const int& power() { return power_; } 
		const int& thunderanti() { return thunderanti_; } 
		const int& maxtp() { return maxtp_; } 
		const int& iceanti() { return iceanti_; } 
		const int& wisdom() { return wisdom_; } 
		const int& defense() { return defense_; } 
		const int& maxhp() { return maxhp_; } 
		const int& fireanti() { return fireanti_; } 
		const int& constitution() { return constitution_; } 
		const int& magdef() { return magdef_; } 
		const int& maxmp() { return maxmp_; } 
		const int& type() { return type_; } 
		const int& quick() { return quick_; } 
		const int& darkanti() { return darkanti_; } 
		
		int power_;
		int thunderanti_;
		int maxtp_;
		int iceanti_;
		int wisdom_;
		int defense_;
		int maxhp_;
		int fireanti_;
		int constitution_;
		int magdef_;
		int maxmp_;
		int type_;
		int quick_;
		int darkanti_;
	};
	typedef std::map<int, Init> InitMap;
	typedef InitMap::iterator InitMapIter;
	typedef InitMap::const_iterator InitMapConstIter;
	InitMap init;
	
	struct Grow {
		bool parse(XMLParser& xml, XMLParser::NodePtr node);
		bool reset();
		
		const int& constitution() { return constitution_; } 
		const int& power() { return power_; } 
		const int& type() { return type_; } 
		const int& maxtp() { return maxtp_; } 
		const int& maxmp() { return maxmp_; } 
		const int& wisdom() { return wisdom_; } 
		const int& defense() { return defense_; } 
		const int& maxhp() { return maxhp_; } 
		const int& quick() { return quick_; } 
		const int& magdef() { return magdef_; } 
		
		int constitution_;
		int power_;
		int type_;
		int maxtp_;
		int maxmp_;
		int wisdom_;
		int defense_;
		int maxhp_;
		int quick_;
		int magdef_;
	};
	typedef std::map<int, Grow> GrowMap;
	typedef GrowMap::iterator GrowMapIter;
	typedef GrowMap::const_iterator GrowMapConstIter;
	GrowMap grow;
	
};
inline bool RoleInitialAbility::parse(XMLParser& xml, XMLParser::NodePtr node) {
	if (!node) return false;
	
	XMLParser::NodePtr init_node = xml.getChildNode(node, "init");
	while (init_node) {
		Init item;
		if (item.parse(xml, init_node))
			init.insert(std::make_pair(item.type_, item));
		init_node = xml.getNextNode(init_node, "init");
	}
	
	XMLParser::NodePtr grow_node = xml.getChildNode(node, "grow");
	while (grow_node) {
		Grow item;
		if (item.parse(xml, grow_node))
			grow.insert(std::make_pair(item.type_, item));
		grow_node = xml.getNextNode(grow_node, "grow");
	}
	return true;
}

inline bool RoleInitialAbility::Init::parse(XMLParser& xml, XMLParser::NodePtr node) {
	if (!node) return false;
	power_ = xml.getNodeProp<int>(node, "power");
	thunderanti_ = xml.getNodeProp<int>(node, "thunderanti");
	maxtp_ = xml.getNodeProp<int>(node, "maxtp");
	iceanti_ = xml.getNodeProp<int>(node, "iceanti");
	wisdom_ = xml.getNodeProp<int>(node, "wisdom");
	defense_ = xml.getNodeProp<int>(node, "defense");
	maxhp_ = xml.getNodeProp<int>(node, "maxhp");
	fireanti_ = xml.getNodeProp<int>(node, "fireanti");
	constitution_ = xml.getNodeProp<int>(node, "constitution");
	magdef_ = xml.getNodeProp<int>(node, "magdef");
	maxmp_ = xml.getNodeProp<int>(node, "maxmp");
	type_ = xml.getNodeProp<int>(node, "type");
	quick_ = xml.getNodeProp<int>(node, "quick");
	darkanti_ = xml.getNodeProp<int>(node, "darkanti");
	return true;
}

inline bool RoleInitialAbility::Grow::parse(XMLParser& xml, XMLParser::NodePtr node) {
	if (!node) return false;
	constitution_ = xml.getNodeProp<int>(node, "constitution");
	power_ = xml.getNodeProp<int>(node, "power");
	type_ = xml.getNodeProp<int>(node, "type");
	maxtp_ = xml.getNodeProp<int>(node, "maxtp");
	maxmp_ = xml.getNodeProp<int>(node, "maxmp");
	wisdom_ = xml.getNodeProp<int>(node, "wisdom");
	defense_ = xml.getNodeProp<int>(node, "defense");
	maxhp_ = xml.getNodeProp<int>(node, "maxhp");
	quick_ = xml.getNodeProp<int>(node, "quick");
	magdef_ = xml.getNodeProp<int>(node, "magdef");
	return true;
}

inline bool RoleInitialAbility::reset() {
	init.clear();
	grow.clear();
	return true;
}

inline bool RoleInitialAbility::Init::reset() {
	power_ = int();
	thunderanti_ = int();
	maxtp_ = int();
	iceanti_ = int();
	wisdom_ = int();
	defense_ = int();
	maxhp_ = int();
	fireanti_ = int();
	constitution_ = int();
	magdef_ = int();
	maxmp_ = int();
	type_ = int();
	quick_ = int();
	darkanti_ = int();
	return true;
}

inline bool RoleInitialAbility::Grow::reset() {
	constitution_ = int();
	power_ = int();
	type_ = int();
	maxtp_ = int();
	maxmp_ = int();
	wisdom_ = int();
	defense_ = int();
	maxhp_ = int();
	quick_ = int();
	magdef_ = int();
	return true;
}

