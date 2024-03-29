#ifndef PILHA_H_INCLUDED
#define PILHA_H_INCLUDED
#include <iostream>

using namespace std;

template <class T>
class Pilha
{
public:
    Pilha();
    ~Pilha();
    void Insere(const T &elemento);
    bool Retira(T &elemento);
    bool EstaVazia() const;

private:
    T *PilhaPtr;
    int Topo;
};

#endif // PILHA_H_INCLUDED
