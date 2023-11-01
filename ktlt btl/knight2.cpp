#include "knight2.h"

Events::Events(const string &file_events)
{
    ifstream fin(file_events);
    fin >> e;
    eventcode = new int[e];
    for (int i = 0; i < e; i++)
    {
        fin >> eventcode[i];
    }
    fin.close();
}

int Events::count() const
{
    return e;
}

int Events::get(int i) const
{
    return eventcode[i];
}

Events::~Events()
{
    delete[] eventcode;
    eventcode = NULL;
    e = 0;
}

bool PhoenixDownI::canUse(BaseKnight *knight)
{
    int hp = knight->getHp();
    int maxHp = knight->getMaxHp();
    if (hp <= 0)
        return true;
    else
        return false;
}

bool PhoenixDownII::canUse(BaseKnight *knight)
{
    int hp{knight->getHp()};
    int maxHp{knight->getMaxHp()};
    if (hp < (maxHp / 4))
        return true;
    else
        return false;
}

bool PhoenixDownIII::canUse(BaseKnight *knight)
{
    int hp{knight->getHp()};
    int maxHp{knight->getMaxHp()};
    if (hp < (maxHp / 3))
        return true;
    else
        return false;
}

bool PhoenixDownIV::canUse(BaseKnight *knight)
{
    int hp{knight->getHp()};
    int maxHp{knight->getMaxHp()};
    if (hp < (maxHp / 2))
        return true;
    else
        return false;
}

void PhoenixDownI::use(BaseKnight *knight)
{
    int hp{knight->getHp()};
    int maxHp{knight->getMaxHp()};
    knight->setHp(maxHp);
    return;
}

void PhoenixDownII::use(BaseKnight *knight)
{
    int hp{knight->getHp()};
    int maxHp{knight->getMaxHp()};
    knight->setHp(maxHp);
    return;
}

void PhoenixDownIII::use(BaseKnight *knight)
{
    int hp{knight->getHp()};
    int maxHp{knight->getMaxHp()};
    if (hp <= 0)
        knight->setHp((maxHp / 3));
    else
        knight->setHp((maxHp / 4) + hp);
    return;
}

void PhoenixDownIV::use(BaseKnight *knight)
{
    int hp{knight->getHp()};
    int maxHp{knight->getMaxHp()};
    if (hp <= 0)
        knight->setHp((maxHp / 2));
    else
        knight->setHp((maxHp / 5) + hp);
    return;
}

/* * * BEGIN implementation of class BaseBag * * */

bool BaseBag::insertFirst(BaseItem *item)
{
    if (count == 0)
    {
        head = new Baglink(item);
        ++count;
        return true;
    }
    else if (maxitem == -1 || count < maxitem)
    {
        Baglink *tmp = new Baglink(item, head);
        head = tmp;
        ++count;
        return true;
    }
    else
        return false;
}

BaseItem *BaseBag::get(ItemType itemType)
{
    Baglink *tmp = head;
    while (tmp)
    {
        if (tmp->item->getType() == itemType && tmp->item->canUse(knight))
        {
            BaseItem *tmpItem = tmp->item;
            tmp->item = head->item;
            head->item = tmpItem;
            return tmpItem;
        }
        tmp = tmp->next;
    }
    return nullptr;
}

BaseItem *BaseBag::getUsableItem()
{
    Baglink *tmp = head;
    while (tmp)
    {
        if (tmp->item->canUse(knight) && tmp->item->getType() != ANTIDOTE && tmp->item != nullptr)
            return tmp->item;
        tmp = tmp->next;
    }
    return nullptr;
}

bool BaseBag::useItem(ItemType itemType)
{
    BaseItem *item = get(itemType);
    if (item)
    {
        item->use(knight);
        Baglink *tmp = head;
        head = head->next;
        --count;
        delete tmp;
        return true;
    }
    return false;
}

string BaseBag::toString() const
{
    string res{"Bag[count="};
    res = res + to_string(count) + ';';
    Baglink *tmp{head};
    while (tmp)
    {
        res = res + tmp->item->toString() + ',';
        tmp = tmp->next;
    }
    if (res[res.length() - 1] == ',')
        res[res.length() - 1] = ']';
    else
        res += ']';

    return res;
}

void BaseBag::drop()
{
    if (count > 0)
    {
        Baglink *tmp = head;
        head = head->next;
        --count;
    }
}

/* * * END implementation of class BaseBag * * */

/* * * BEGIN implementation of class BaseKnight * * */

bool Prime(int n)
{
    if (n < 2)
        return false;
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
        {
            return false;
            break;
        }
    }
    return true;
}

bool Pythago(int n)
{
    int a = n % 10;
    int b = (n / 10) % 10;
    int c = n / 100;
    if (a * b * c == 0)
        return false;
    else if (a * a + b * b == c * c)
        return true;
    else if (b * b + c * c == a * a)
        return true;
    else if (c * c + a * a == b * b)
        return true;
    return false;
}

BaseKnight *BaseKnight::create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
{
    BaseKnight *knight{nullptr};
    if (maxhp == 888)
    {
        knight = new LancelotKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    }
    else if (Prime(maxhp))
    {
        knight = new PaladinKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    }
    else if (maxhp > 99 && maxhp <= 999 && Pythago(maxhp))
    {
        knight = new DragonKnight(id, maxhp, level, gil, 0, phoenixdownI);
    }
    else
    {
        knight = new NormalKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    }
    return knight;
}

bool BaseKnight::HpDown()
{
    BaseItem *usableItem = bag->getUsableItem();
    if (usableItem && bag->useItem(usableItem->getType()))
    {
        return true;
    }
    else if (this->hp <= 0 && this->gil >= 100)
    {
        this->hp = this->maxhp / 2;
        this->gil -= 100;
        return true;
    }
    return false;
}

void PaladinKnight::fight(BaseOpponent *opponent)
{
    if (opponent->getType() <= 5)
    {
        wincreep(opponent);
    }
    else if (this->level >= opponent->getLevel())
    {
        if (opponent->getType() == 6)
        {
            winTornbery();
        }
        else
            winQueen();
    }
    else if (opponent->getType() == 6)
    {
        loseTornbery();
    }
}

void LancelotKnight::fight(BaseOpponent *opponent)
{

    if (opponent->getType() <= 5)
    {
        wincreep(opponent);
    }
    else if (this->level >= opponent->getLevel())
    {
        if (opponent->getType() == 6)
        {
            winTornbery();
        }
        else
            winQueen();
    }
    else if (opponent->getType() == 6)
    {
        loseTornbery();
    }
    else
        loseQueen();
}

void DragonKnight::fight(BaseOpponent *opponent)
{

    if (this->level >= opponent->getLevel())

    {
        if (opponent->getType() <= 5)
            wincreep(opponent);
        else if (opponent->getType() == 6)
        {
            winTornbery();
        }
        else
            winQueen();
    }
    else if (opponent->getType() <= 5)
    {
        losecreep(opponent);
    }
    else if (opponent->getType() == 7)
        loseQueen();
}

void NormalKnight::fight(BaseOpponent *opponent)
{

    if (this->level >= opponent->getLevel())
    {
        if (opponent->getType() <= 5)
            wincreep(opponent);
        else if (opponent->getType() == 6)
        {
            winTornbery();
        }
        else
            winQueen();
    }
    else if (opponent->getType() <= 5)
    {
        losecreep(opponent);
    }
    else if (opponent->getType() == 6)
        loseTornbery();
    else
        loseQueen();
}

string BaseKnight::toString() const
{
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    // inefficient version, students can change these code
    //      but the format output must be the same
    string s("");
    s += "[Knight:id:" + to_string(id) + ",hp:" + to_string(hp) + ",maxhp:" + to_string(maxhp) + ",level:" + to_string(level) + ",gil:" + to_string(gil) + "," + bag->toString() + ",knight_type:" + typeString[knightType] + "]";
    return s;
}

/* * * END implementation of class BaseKnight * * */

/* * * BEGIN implementation of class ArmyKnights * * */
void ArmyKnights::printInfo() const
{
    cout << "No. knights: " << this->count();
    if (this->count() > 0)
    {
        BaseKnight *lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
         << ";LancelotSpear:" << this->hasLancelotSpear()
         << ";GuinevereHair:" << this->hasGuinevereHair()
         << ";ExcaliburSword:" << this->hasExcaliburSword()
         << endl
         << string(50, '-') << endl;
}

void ArmyKnights::printResult(bool win) const
{
    cout << (win ? "WIN" : "LOSE") << endl;
}

ArmyKnights::ArmyKnights(const string &file_armyknights) : obtained{}
{
    ifstream finn(file_armyknights);
    finn >> n;
    n0 = n;
    knight = new BaseKnight *[n0];
    for (int i = 0; i < n0; ++i)
    {
        int hp, level, phoenixdownI, gil, antidote;
        finn >> hp >> level >> phoenixdownI >> gil >> antidote;
        knight[i] = BaseKnight::create(i + 1, hp, level, gil, antidote, phoenixdownI);
    }
    finn.close();
}

ArmyKnights::~ArmyKnights()
{
    for (int i = 0; i < n0; i++)
    {
        delete knight[i];
    }
    delete[] knight;
    knight = nullptr;
    n = 0;
}

int ArmyKnights::count() const
{
    return n;
}

BaseKnight *ArmyKnights::lastKnight() const
{
    if (n == 0)
    {
        return NULL;
    }
    if (obtained.ultimecia)
    {
        for (int i = n0 - 1; i >= 0; i--)
        {
            if (knight[i] != nullptr)
                return knight[i];
        }
    }
    return knight[n - 1];
}

void ArmyKnights::removeknight(int i)
{
    if (knight && knight[i])
    {
        delete knight[i];
        knight[i] = nullptr;
        --n;
    }
}

void ArmyKnights::passGil(int gil, int i)
{
    while (i >= 0 && gil > 0)
    {
        int Gil = knight[i]->getGil();
        Gil += gil;
        gil = 0;
        if (Gil > 999)
        {
            gil = Gil - 999;
            Gil = 999;
        }
        knight[i]->setGil(Gil);
        --i;
    }
}

bool ArmyKnights::adventure(Events *events)
{
    bool result = true;
    int e = events->count();
    for (int i = 0; i < e; i++)
    {
        int eventcode = events->get(i);
        int level0 = (i + eventcode) % 10 + 1;
        if (eventcode < 12)
        {
            int gil;
            int bd;
            BaseOpponent *opponent;
            if (eventcode == 1)
            {
                opponent = new MadBear(level0, 1, 100, 10);
            }
            if (eventcode == 2)
            {
                opponent = new Bandit(level0, 2, 150, 15);
            }
            if (eventcode == 3)
            {
                opponent = new LordLupin(level0, 3, 450, 45);
            }
            if (eventcode == 4)
            {
                opponent = new Elf(level0, 4, 750, 75);
            }
            if (eventcode == 5)
            {
                opponent = new Troll(level0, 5, 800, 95);
            }
            if (eventcode == 6)
            {
                opponent = new Tornbery(level0, 6, 0, 0);
            }
            if (eventcode == 7)
            {
                opponent = new QueenOfCards(level0, 7, 0, 0);
            }
            if (eventcode == 8)
            {
                opponent = new NinaDeRings(0, eventcode, 0, 0);
            }
            if (eventcode == 9)
            {
                opponent = new DurianGarden(0, eventcode, 0, 0);
            }
            if (eventcode == 10)
            {
                opponent = new OmegaWeapon(0, eventcode, 0, 0);
            }
            if (eventcode == 11)
            {
                opponent = new Hades(0, eventcode, 0, 0);
            }
            bool result = fight(opponent);
        }
        else if (eventcode == 95)
        {
            obtained.shield = true;
            result = true;
        }
        else if (eventcode == 96)
        {
            obtained.spear = true;
            result = true;
        }
        else if (eventcode == 97)
        {
            obtained.hair = true;
            result = true;
        }
        else if (eventcode == 98)
        {
            obtained.sword = obtained.hair && obtained.spear && obtained.shield;
            result = true;
        }
        else if (eventcode == 99)
        {
            if (obtained.sword)
            {
                result = true;
            }
            else if (!hasPaladinShield() || !hasLancelotSpear() || !hasGuinevereHair())
            {
                for (int j = n0 - 1; j >= 0; --j)
                    removeknight(j);
                bool result = false;
            }
            else
            {
                bool result = false;
                int iniHp = 5000;
                for (int j = n - 1; j >= 0; j--)
                {
                    if (knight[j]->getType() != NORMAL)
                    {
                        int damage = (int)(knight[j]->getLv() * knight[j]->getHp() * knight[j]->baseDameknight);
                        iniHp -= damage;
                        if (iniHp <= 0)
                        {
                            bool result = true;
                            obtained.ultimecia = true;
                            break;
                        }

                        removeknight(j);
                    }
                }
                if (iniHp > 0)
                {
                    for (int k = n0 - 1; k >= 0; k--)
                        removeknight(k);
                }
            }
        }
        else
        {
            takePhoenix(eventcode);
            result = true;
        }
        if (n <= 0)
        {
            result = false;
            printInfo();
            break;
        }

        printInfo();
    };
    return result;
}

void ArmyKnights::takePhoenix(int type)
{
    BaseItem *tmp = nullptr;
    if (type == 112)
        tmp = new PhoenixDownII;
    if (type == 113)
        tmp = new PhoenixDownIII;
    if (type == 114)
        tmp = new PhoenixDownIV;
    for (int i = n - 1; i >= 0; i--)
    {
        if (knight[i]->getBag()->insertFirst(tmp))
            return;
    }
    delete tmp;
    return;
}

bool ArmyKnights::fight(BaseOpponent *opponent)
{
    BaseKnight *lknight = this->lastKnight();
    int egil = opponent->getGil();
    int bd = opponent->getDame();
    int eventcode = opponent->getType();
    int level0 = opponent->getLevel();
    if (eventcode <= 5)
    {
        lknight->fight(opponent);
        if (lknight->getHp() <= 0)
            removeknight(n - 1);
        else
        {
            int gil = lknight->getGil();
            if (gil > 999)
            {
                lknight->setGil(999);
                passGil(gil - 999, n - 2);
            }
        }
    }
    if (eventcode == 6)

    {
        lknight->fight(opponent);
    }

    if (eventcode == 7)
    {

        lknight->fight(opponent);
        int gil = lknight->getGil();
        if (gil > 999)
        {
            lknight->setGil(999);
            passGil(gil - 999, n - 2);
        }
    }

    if (eventcode == 8)
    {

        int Gil = lknight->getGil();
        int Hp = lknight->getHp();
        int maxHp = lknight->getMaxHp();
        if (lknight->getType() == PALADIN && Hp < (maxHp / 3))
        {
            lknight->setHp(Hp + maxHp / 5);
        }
        else if (Gil >= 50 && Hp < (maxHp / 3))
        {
            lknight->setGil(Gil - 50);
            lknight->setHp(Hp + maxHp / 5);
        }
    }

    if (eventcode == 9)
    {

        lknight->setHp(lknight->getMaxHp());
    }

    if (eventcode == 10)
    {
        if (obtained.omega)
            return true;
        if (lknight->getType() == DRAGON ||
            (lknight->getLv() == 10 && lknight->getHp() == lknight->getMaxHp()))
        {
            obtained.omega = true;
            lknight->setLv(10);
            lknight->setGil(999);
            return true;
        }
        else
        {
            lknight->setHp(0);
            lknight->HpDown();
            if (lknight->getHp() <= 0)
                removeknight(n - 1);
            else
                return true;
        }
    }
    if (eventcode == 11)
    {
        if (obtained.hades)
            return true;
        if (lknight->getLv() == 10 ||
            lknight->getType() == PALADIN && lknight->getLv() > 7)
        {
            obtained.hades = true;
            obtained.shield = true;
            return true;
        }
        else
        {
            lknight->setHp(0);
            lknight->HpDown();
            if (lknight->getHp() <= 0)
                removeknight(n - 1);
        }
    }

    delete opponent;
    if (n <= 0)
        return false;
    return true;
}

/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class KnightAdventure * * */
KnightAdventure::KnightAdventure()
{
    armyKnights = nullptr;
    events = nullptr;
}

KnightAdventure::~KnightAdventure()
{
    if (armyKnights)
        delete armyKnights;
    if (events)
        delete events;
}

void KnightAdventure::loadArmyKnights(const string &finn)
{
    armyKnights = new ArmyKnights(finn);
}

void KnightAdventure::loadEvents(const string &fin)
{
    events = new Events(fin);
}

void KnightAdventure::run()
{
    if (armyKnights && events)
    {
        bool knight = armyKnights->adventure(events);
        armyKnights->printResult(knight);
    }
}
/* * * END implementation of class KnightAdventure * * */