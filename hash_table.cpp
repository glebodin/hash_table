#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <map>
using namespace std;
const int MAXSIZE = 100000;

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> > class HashMap {

public:
    typedef typename list<pair<const KeyType, ValueType>>::iterator iterator;
    typedef typename list<pair<const KeyType, ValueType>>::const_iterator const_iterator;
    typedef pair<const KeyType, ValueType> cell;

private :
    list<cell> Data;
    vector<list<pair<const KeyType, iterator> > > GetByKey;
    size_t TableSize = MAXSIZE;
    Hash Hasher;

public:

    bool empty() const{
        return this->Data.size() == 0;
    }

    size_t size() const{
        return this->Data.size();
    }

    Hash hash_function() const {
        return this->Hasher;
    }

    HashMap(Hash h = Hash{}) : GetByKey(MAXSIZE), Hasher(h) {}

    iterator begin() {
        return Data.begin();
    }

    iterator end() {
        return Data.end();
    }

    const_iterator begin() const{
        return Data.cbegin();
    }

    const_iterator end() const{
        return Data.cend();
    }

    void clear() {
        for (auto tmp : Data) {
            GetByKey[Hasher(tmp.first % TableSize)].clear();
        }
        Data.clear();
    }

    HashMap& operator=(const HashMap a) {
        this->clear();
        this->TableSize = a.TableSize;
        for (auto& to : a.Data) {
            this->Data.push_back(to);
        }
        this->Hasher = a.Hasher;
        fillTable();
        return *this;
    }

    template<typename Iterator>
    HashMap(Iterator begin, Iterator end, Hash h = Hash{}) {
        GetByKey.resize(MAXSIZE);
        Hasher = h;
        for (iterator a = begin; a != end; a++) {
            Data.push_back(*a);
        }
        fillTable();
    }

    HashMap(std::initializer_list<pair<const KeyType, ValueType> > a, Hash h = Hash{}) {
        GetByKey.resize(MAXSIZE);
        Hasher = h;
        for (auto to : a) {
            this->Data.push_back(to);
        }
        fillTable();
    }

    void fillTable() {
        for (iterator it = Data.begin(); it != Data.end(); it++) {
            GetByKey[Hasher(it->first) % (this->TableSize)].push_back({it->first, it});
        }
    }

    const_iterator find(const KeyType i) const {
        for (auto to : GetByKey[Hasher(i) % this->TableSize]) {
            if (to.first == i) {
                return to.second;
            }
        }
        return Data.cend();
    }

    void insert(cell need) {
        if (find(need.first) != Data.end()) {
            return;
        }
        Data.push_back(need);
        GetByKey[Hasher(need.first) % this->TableSize].push_back(make_pair(need.first, (--Data.end())));
    }

    void erase(const KeyType i) {
        for (auto it = GetByKey[Hasher(i) % this->TableSize].begin(); it != GetByKey[Hasher(i) % this->TableSize].end(); it++) {
            auto to = *it;
            if (to.first == i) {
                Data.erase(to.second);
                GetByKey[Hasher(i) % this->TableSize].erase(it);
                return;
            }
        }
    }

    ValueType& operator[] (const KeyType i) {
        for (auto& to : GetByKey[Hasher(i) % this->tTbleSize]) {
            if (to.first == i) {
                return (to.second)->second;
            }
        }
        insert({i, ValueType()});
        return (--Data.end())->second;
    }

    const ValueType& at (const KeyType i) const{
        for (auto& to : GetByKey[Hasher(i) % this->TableSize]) {
            if (to.first == i) {
                return (to.second)->second;
            }
        }
        throw std::out_of_range("-_-");
    }

    iterator find(const KeyType i) {
        for (auto& to : GetByKey[Hasher(i) % this->TableSize]) {
            if ((to.second)->first == i) {
                return to.second;
            }
        }
        return Data.end();
    }
};
