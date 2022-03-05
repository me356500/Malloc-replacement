/*
Student No.: 0716321
Student Name: 魏翌丞
Email: jlxuugu2@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/
// cd /home/w/Desktop/OS/hw4
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
struct block {
    size_t size;
    int free;
    struct block *prev;
    struct block *next;
    
};
struct block *first = NULL;

//first malloc 
//gcc -shared -o ff.so -fPIC ff.c
//mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
//unsigned char *ADDR
void *malloc(size_t size) {
    char *text = (char*)
    mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    memset(text,0,1000);
    if(size == 0) {    
        size_t sum = 0;
        struct block *cs = first;
        while(cs != NULL){            
            //snprintf(text, 150,"%ldin  %p  f:%d  cs:%ld \n",cs-first,cs, cs->free, cs->size);
            //write(1, text, 150);
            //memset(text,0,1000);
            if(cs->free == 1)
                if(cs->size > sum)
                    sum = cs->size;
            cs = cs->next;
        }
        //minus header size
        snprintf(text, 35,"Max Free Chunk Size = %ld\n", sum - 32);
        write(1, text, 35);
        memset(text, 0, 1000);
        munmap(text, 1000);
        munmap(first, 20000);
        return 0;
    }
    float a = size, b = 32;
    float r = a / b;
    size_t mul = size / 32;
    
    if(r > (int)r)
        size = (32 * (mul + 1));
    else
        size = (32 * mul);
    
    
    //memset(text,0,17);
    if(first == NULL){
        first = (struct block *)
        mmap(NULL, 20000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
        if(first == MAP_FAILED){
            snprintf(text, 15,"map failed\n");
            write(1, text, 15);
            return 0;
        }
        first[0].size = 20000;
        first[0].free = 1;
        first[0].prev = NULL;
        first[0].next = NULL;  
    }
    //snprintf(text, 80,"%p  %p  %p  %d\n",first, first+1,&first[1],first[0].free);
    //write(1, text, 80);
    //memset(text,0,1000);
    struct block *current = &first[0];
    struct block *bf = NULL;
    size_t bfinc = 0;
    size_t c = 1;

    size += 32;
    while(current != NULL){
        
        //snprintf(text, 100,"%ld\tin  %p  f:%d  cs:%ld s:%ld\n",current-first,current, current->free, current->size, size);
        //write(1, text, 100);
        //memset(text,0,1000);
        
        if(current->free == 1 && current->size >= size) {
            size_t inc = c / 32 + size / 32;
            size_t r = current->size - size;
            
            if(r == 0) {
                current->free = 0;
                munmap(text, 1000);
                return (current + 1);
            }
            if(bf == NULL) {
                bf = current;
                bfinc = inc;
                
            }
            else if(bf->size > current->size) {
                bf = current;
                bfinc = inc;
            }
  
        }        
        
        c += current->size;
        
        current = current->next;
        
    }
    
    struct block *tnext = bf->next;
    struct block *temp = &first[bfinc]; 
           
    size_t rest = bf->size - size ;
    
    bf->free = 0;
    bf->size = size;
    bf->next = temp;
            

    temp->free = 1;
    temp->prev = bf;
    temp->next = tnext;
    temp->size = rest;

    if(tnext != NULL)
        tnext->prev = temp;
    /*
        snprintf(text, 80,"test SE\n");
        write(1, text, 80);
        memset(text,0,1000);
    */
    munmap(text, 1000);

    return (bf + 1);
    
}
void free(void *ptr) {
    struct block *current = first;
    while(1) {
        struct block *n = current->next;
        struct block *p = current->prev;
        if(current + 1 == ptr) {
            //merge right
            while(n) {
                if(n->free) {
                    current->size += n->size  ;
                    current->free = 1;
                    n->size = 0;
                    current->next = n->next;
                    if(n->next)
                        n->next->prev = current;
                    n = n->next;
                }
                else {
                    break;
                }
            }
            //merge left
            while(p){
                if(p->free) {
                    p->size += current->size;
                    current->free = 1;
                    current->size = 0;
                    p->next = current->next;
                    if(current->next)
                        current->next->prev = p;
                    current = p;
                    p = p->prev;
                    
                }
                else {
                    break;
                }
            }
            //only free itself
            current->free = 1;



            break;
        }
        current = current->next;
    }

}
