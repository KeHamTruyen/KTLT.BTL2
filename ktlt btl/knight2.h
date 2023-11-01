#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG

class BaseItem;
class PhoenixDownI;
class PhoenixDownII;
class PhoenixDownIII;
class PhoenixDownIV;
class Antidote;
class BaseKnight;
class Events;

enum ItemType
{
    ANTIDOTE = 0,
    PHOENIXDOWNI,
    PHOENIXDOWNII,
    PHOENIXDOWNIII,
    PHOENIXDOWNIV
};

class BaseItem
{
protected:
    ItemType itemType;

public:
    virtual ItemType getType() { return itemType; }
    virtual string toString() = 0;
    virtual bool canUse(BaseKnight *knight) = 0;
    virtual void use(BaseKnight *knight) = 0;
    virtual ~BaseItem(){};
};

class Antidote : public BaseItem
{
public:
    Antidote()
    {
        this->itemType = ANTIDOTE;
    }

    ~Antidote() {}

    string toString() { return "Antidote"; }

    bool canUse(BaseKnight *knight)
    {
        return true;
    }

    void use(BaseKnight *knight)
    {
        return;
    }
};

class PhoenixDownI : public BaseItem
{
public:
    PhoenixDownI()
    {
        this->itemType = PHOENIXDOWNI;
    }

    ~PhoenixDownI() {}

    string toString() { return "PhoenixI"; }

    bool canUse(BaseKnight *knight);

    void use(BaseKnight *knight);
};

class PhoenixDownII : public BaseItem
{
public:
    PhoenixDownII()
    {
        this->itemType = PHOENIXDOWNII;
    }

    ~PhoenixDownII() {}

    string toString() { return "PhoenixII"; }

    bool canUse(BaseKnight *knight);

    void use(BaseKnight *knight);
};

class PhoenixDownIII : public BaseItem
{
public:
    PhoenixDownIII()
    {
        this->itemType = PHOENIXDOWNIII;
    }

    ~PhoenixDownIII() {}

    string toString() { return "PhoenixIII"; }

    bool canUse(BaseKnight *knight);

    void use(BaseKnight *knight);
};

class PhoenixDownIV : public BaseItem
{
public:
    PhoenixDownIV()
    {
        this->itemType = PHOENIXDOWNIV;
    }

    ~PhoenixDownIV() {}

    string toString() { return "PhoenixIV"; }

    bool canUse(BaseKnight *knight);

    void use(BaseKnight *knight);
};

class BaseBag
{
    struct Baglink
    {
        BaseItem *item;
        Baglink *next;
        Baglink(BaseItem *item, Baglink *nextptr = nullptr) : item(item), next(nextptr) {}
        ~Baglink()
        {
            if (item)
                delete item;
        }
    };
    Baglink *head;
    int count;
    int maxitem;

protected:
    BaseKnight *knight;

public:
    BaseBag(BaseKnight *knight, int phoenixdownI, int antidote) : knight(knight), maxitem(-1), count{0}, head(nullptr)
    {

        for (int i = 0; i < phoenixdownI; i++)
        {
            BaseItem *item = new PhoenixDownI();
            insertFirst(item);
        }

        for (int i = 0; i < antidote; i++)
        {
            BaseItem *item = new Antidote();
            insertFirst(item);
        }
    };
    ~BaseBag()
    {
        while (head)
        {
            Baglink *tmp{head};
            head = head->next;
            delete tmp;
        }
        head = nullptr;
        count = 0;
    }
    virtual bool insertFirst(BaseItem *item);
    virtual BaseItem *get(ItemType itemType);
    virtual string toString() const;
    virtual bool useItem(ItemType itemType);
    virtual void drop();
    virtual BaseItem *getUsableItem();
};

class PaladinBag : public BaseBag
{
    int maxitem = -1;

public:
    PaladinBag(BaseKnight *knight, int phoenixdownI, int antidote) : BaseBag(knight, phoenixdownI, antidote)
    {
    }
};

class LanceLotBag : public BaseBag
{
    int maxitem = 16;

public:
    LanceLotBag(BaseKnight *knight, int phoenixdownI, int antidote) : BaseBag(knight, phoenixdownI, antidote)
    {
    }
};

class DragonBag : public BaseBag
{
    int maxitem = 14;

public:
    DragonBag(BaseKnight *knight, int phoenixdownI, int antidote) : BaseBag(knight, phoenixdownI, antidote)
    {
    }
};

class NormalBag : public BaseBag
{
    int maxitem = 14;

public:
    NormalBag(BaseKnight *knight, int phoenixdownI, int antidote) : BaseBag(knight, phoenixdownI, antidote)
    {
    }
};

class BaseOpponent
{
protected:
    int level0;
    int eventcode;
    int gil;
    int baseDamage;

public:
    BaseOpponent(int level0, int eventcode, int gil, int baseDamage) : level0(level0), eventcode(eventcode), gil(gil), baseDamage(baseDamage)
    {
    }
    int getType() { return eventcode; };
    int getLevel() { return level0; };
    int getGil() { return gil; };
    int getDame() { return baseDamage; };
};

class MadBear : public BaseOpponent
{
public:
    MadBear(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(level0, 1, 100, 10){};
};

class Bandit : public BaseOpponent
{
public:
    Bandit(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(level0, 2, 150, 15){};
};

class LordLupin : public BaseOpponent
{
public:
    LordLupin(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(level0, 3, 450, 45){};
};

class Elf : public BaseOpponent
{
public:
    Elf(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(level0, 4, 750, 75){};
};

class Troll : public BaseOpponent
{
public:
    Troll(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(level0, 5, 800, 95){};
};

class Tornbery : public BaseOpponent
{
public:
    Tornbery(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(level0, 6, 0, 0){};
};

class QueenOfCards : public BaseOpponent
{
public:
    QueenOfCards(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(level0, 7, 0, 0){};
};

class NinaDeRings : public BaseOpponent
{
public:
    NinaDeRings(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(0, 8, 0, 0){};
};

class DurianGarden : public BaseOpponent
{
public:
    DurianGarden(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(0, 9, 0, 0){};
};

class OmegaWeapon : public BaseOpponent
{
public:
    OmegaWeapon(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(0, 10, 0, 0){};
};

class Hades : public BaseOpponent
{
public:
    Hades(int level0, int eventcode, int gil, int baseDamage) : BaseOpponent(0, 11, 0, 0){};
};

enum KnightType
{
    PALADIN = 0,
    LANCELOT,
    DRAGON,
    NORMAL
};

class BaseKnight
{
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;
    BaseBag *bag;
    KnightType knightType;

public:
    static BaseKnight *create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    string toString() const;
    double baseDameknight;
    virtual ~BaseKnight()
    {
        if (bag)
            delete bag;
        bag = nullptr;
    }
    int getHp() const { return hp; };
    void setHp(int hp) { this->hp = hp; };
    int getMaxHp() const { return maxhp; };
    int getLv() const { return level; };
    void setLv(int level) { this->level = level; };
    KnightType getType() const { return knightType; };
    int getGil() const { return gil; };
    void setGil(int gil) { this->gil = gil; };
    BaseBag *getBag() { return bag; };
    virtual bool HpDown();
    virtual void fight(BaseOpponent *opponent) = 0;

protected:
    virtual void wincreep(BaseOpponent *opponent)
    {
        this->gil += opponent->getGil();
    }
    virtual void losecreep(BaseOpponent *opponent)
    {
        int damage = opponent->getDame() * (opponent->getLevel() - this->level);
        this->hp -= damage;
        HpDown();
    }
    virtual void winTornbery()
    {
        this->level += 1;
        if (this->level > 10)
            this->level = 10;
    }
    virtual void loseTornbery()
    {
        if (antidote > 0)
        {
            --antidote;
            bag->useItem(ANTIDOTE);
        }
        else
        {
            for (int i = 0; i < 3; i++)
                bag->drop();
            this->hp -= 10;
            HpDown();
        }
    }
    virtual void winQueen()
    {
        this->gil *= 2;
    }
    virtual void loseQueen()
    {
        this->gil /= 2;
    }
};

class PaladinKnight : public BaseKnight
{
public:
    PaladinKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new PaladinBag(this, phoenixdownI, antidote);
        this->knightType = PALADIN;
        baseDameknight = 0.06;
    }
    ~PaladinKnight()
    {
        if (bag)
            delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent *opponent);
};

class LancelotKnight : public BaseKnight
{
public:
    LancelotKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new LanceLotBag(this, phoenixdownI, antidote);
        this->knightType = LANCELOT;
        baseDameknight = 0.05;
    }
    ~LancelotKnight()
    {
        if (bag)
            delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent *opponent);
};

class DragonKnight : public BaseKnight
{
public:
    DragonKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new DragonBag(this, phoenixdownI, antidote);
        this->knightType = DRAGON;
        baseDameknight = 0.075;
    }
    ~DragonKnight()
    {
        if (bag)
            delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent *opponent);
};

class NormalKnight : public BaseKnight
{
public:
    NormalKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new NormalBag(this, phoenixdownI, antidote);
        this->knightType = NORMAL;
        baseDameknight = 0;
    }
    ~NormalKnight()
    {
        if (bag)
            delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent *opponent);
};

class DoiQuanHiepSi
{
public:
    bool shield;
    bool spear;
    bool hair;
    bool sword;
    bool omega;
    bool hades;
    bool ultimecia;
    DoiQuanHiepSi()
    {
        shield = false;
        spear = false;
        hair = false;
        sword = false;
        omega = false;
        hades = false;
        ultimecia = false;
    }
};

class ArmyKnights
{
    BaseKnight **knight;
    int n0;
    int n;
    DoiQuanHiepSi obtained;

public:
    ArmyKnights(const string &file_armyknights);
    ~ArmyKnights();
    bool fight(BaseOpponent *opponent);
    bool adventure(Events *events);
    int count() const;
    BaseKnight *lastKnight() const;

    bool hasPaladinShield() const { return obtained.shield; };
    bool hasLancelotSpear() const { return obtained.spear; };
    bool hasGuinevereHair() const { return obtained.hair; };
    bool hasExcaliburSword() const { return obtained.sword; };

    void printInfo() const;
    void printResult(bool win) const;
    void takePhoenix(int type);

private:
    void removeknight(int i);

    void passGil(int, int);
};

class Events
{
private:
    int e;
    int *eventcode;

public:
    Events(const string &file_events);
    int count() const;
    int get(int i) const;
    ~Events();
};

class KnightAdventure
{
private:
    ArmyKnights *armyKnights;
    Events *events;

public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:

    void loadArmyKnights(const string &);
    void loadEvents(const string &);
    void run();
};

#endif // __KNIGHT2_H__