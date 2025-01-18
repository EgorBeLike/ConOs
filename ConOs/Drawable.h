#pragma once
#ifndef CONOS_DRAWABLE
#define CONOS_DRAWABLE 1

#include "Define.h"
#include "Logger.h"

struct Drawable {
    Parent* elem;
    sf::Drawable* drawable;
    size_t layer;
    Drawable(Parent* p, sf::Drawable* d, size_t l) : elem(p), drawable(d), layer(l) {}
};

class DrawableSorter : public Parent {
public:
    using List = vector<Drawable*>;
protected:
    List unsorted;
    List sorted;

    List SortLayer(List unsrt) {
        List srt = unsrt;
        for (size_t i = 0; i < srt.size() - 1; i++) {
            if (srt[i]->layer > srt[i + 1]->layer) {
                swap(srt[i], srt[i + 1]);
            }
        }
        return srt;
    }
    bool CheckLayer(List srt) {
        for (size_t i = 0; i < srt.size() - 1; i++) {
            if (srt[i]->layer > srt[i + 1]->layer) {
                return false;
            }
        }
        return true;
    }

public:
    void Sort() {
        if (unsorted.size() <= 1) { sorted = unsorted; return; }
        List unsrt = unsorted;
        while (!CheckLayer(unsrt)) {
            unsrt = SortLayer(unsrt);
        }
        sorted = unsrt;
    }

    DrawableSorter(Logger* l, List unsrt, string name = "DrawableSorter-Worker") : Parent(l, name) {
        unsorted = unsrt;
        Sort();
    }

    DrawableSorter(Logger* l, string name = "DrawableSorter-Worker") : Parent(l, name) {
        unsorted = List();
    }
    class iterator {
    protected:
        DrawableSorter* parent;
        Drawable* value;
        size_t pos;
        inline size_t getSafePos(size_t rp) {
            if (parent->last() > rp) return parent->last();
            if (0 > rp) return 0;
            return rp;
        }
    public:
        size_t getPos() const { return pos; }
        iterator(DrawableSorter* sorter, size_t position) {
            parent = sorter;
            pos = position;
            value = parent->operator[](pos);
        }
        Drawable*& operator*() {
            return value;
        }
        Drawable** operator->() {
            return &value;
        }
        bool operator==(iterator& r) {
            return (parent == r.parent) && (pos == r.pos);
        }
        iterator& setPos(size_t p) {
            size_t _p = getSafePos(p);
            if (_p != p) {
                parent->logger->SendSignal(parent, ERR, "iterator.operator+=: arg \"p\" isn't valid. Setted valid pos");
                p = _p;
            }
            pos = p;
            value = parent->operator[](pos);
            return *this;
        }
        iterator& operator+=(size_t p) {
            return setPos(pos + p);
        }
        iterator& operator-=(size_t p) {
            return setPos(pos - p);
        }
        iterator& operator++() {
            return setPos(pos + 1);
        }
        iterator operator++(int) {
            iterator _ = *this;
            setPos(pos + 1);
            return _;
        }
        iterator& operator--() {
            return setPos(pos - 1);
        }
        iterator operator--(int) {
            iterator _ = *this;
            setPos(pos - 1);
            return _;
        }
    };
    DrawableSorter& operator+=(Drawable* d) {
        unsorted.push_back(d);
        Sort();
        return *this;
    }
    DrawableSorter& operator-=(size_t pos) {
        size_t i = 0;
        for (List::iterator iter = unsorted.begin(); iter != unsorted.end(); iter++) {
            if (i == pos) {
                unsorted.erase(iter);
                break;
            }
            i++;
        }
        return *this;
    }
    DrawableSorter& operator-=(iterator it) {
        size_t i = 0;
        for (List::iterator iter = unsorted.begin(); iter != unsorted.end(); iter++) {
            if (i == it.getPos()) {
                unsorted.erase(iter);
                break;
            }
            i++;
        }
        return *this;
    }
    DrawableSorter& operator-=(List::iterator iter) {
        unsorted.erase(iter);
        Sort();
        return *this;
    }
    DrawableSorter& operator--() {
        unsorted.erase(unsorted.end());
        Sort();
        return *this;
    }
    DrawableSorter operator--(int) {
        DrawableSorter _ = *this;
        unsorted.erase(unsorted.end());
        Sort();
        return _;
    }
    Drawable* operator[](size_t pos) {
        return sorted[pos];
    }
    List& operator*() {
        return sorted;
    }
    List* operator->() {
        return &sorted;
    }
    size_t size() {
        return sorted.size();
    }
    size_t last() {
        return sorted.size() - 1;
    }
    iterator begin() {
        return iterator(this, 0);
    }
    iterator end() {
        return iterator(this, last());
    }
    iterator iter(size_t p) {
        return iterator(this, p);
    }
    void Main() {
        List ounsrt = unsorted;
        while (work) {
            if (ounsrt != unsorted) Sort();
        }
        Exit();
    }
};

#endif