# buddy_malloc
Implementazione di malloc e free utilizzando un buddy allocator con bitmap
# MyAlloc
MyAlloc è una libreria di allocazione della memoria per C che utilizza un algoritmo di allocazione binaria buddy per gestire la memoria in modo efficiente.

# Istruzioni
## Compilazione
Viene fornito un makefile per facilitare la compilazione, basta digitare nel terminale
```
make
```
## Utilizzo
Per utilizzare MyAlloc, è necessario includere il file di intestazione myalloc.h nel proprio codice C e utilizzare le funzioni myalloc e myfree per allocare e liberare la memoria, rispettivamente.
```
#include "myalloc.h"

int main() {
    // Allocazione di un blocco di memoria di 100 byte
    void* ptr = myalloc(100);

    // Liberazione del blocco di memoria
    myfree(ptr);

    return 0;
}
```
# Come funziona
MyAlloc utilizza un algoritmo di allocazione binaria buddy per gestire la memoria. Questo algoritmo funziona dividendo i blocchi di memoria liberi in due parti uguali finché non viene trovato un blocco della dimensione desiderata. Se non è disponibile un blocco della dimensione desiderata, l'algoritmo cercherà di dividere un blocco più grande in due parti e ripetere la ricerca.

MyAlloc tiene traccia dei blocchi di memoria liberi utilizzando una astrazione della bitmap su un albero. Quando viene richiesto un blocco di memoria, MyAlloc cerca un blocco della dimensione desiderata. Se viene trovato un blocco della dimensione desiderata, viene restituito al chiamante. In caso contrario, MyAlloc cercherà di dividere un blocco più grande in due parti e ripetere la ricerca.

Quando un blocco di memoria viene liberato, MyAlloc lo aggiunge alla lista concatenata dei blocchi di memoria liberi. Se un blocco di memoria libero confina con un altro blocco di memoria libero, MyAlloc unirà i due blocchi in un unico blocco più grande.
