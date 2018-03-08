#include "bitset.h"

bitSet<int> final[1431];

enum{AND, OR, AND_INIT, OR_INIT};
Result * DivResultSort(Result *head){
    Result *first, *t, *q, *p;
    first = head->next;
    head->next = NULL;

    while (first != NULL)
    {

        for (t = first, q = head; ((q != NULL) && (q->PageRank > t->PageRank)); p = q, q = q->next);


        first = first->next;

        if (q == head)
        {
            head = t;
        }
        else
        {
            p->next = t;
        }
        t->next = q;
    }
    return head;
}
void DivideResult(Result * resultHead, int type){
    Result *DivResult1 = NULL;
    Result *DivResult2 = NULL;

    Result *t = resultHead;

    while(t != NULL){
        Result *p = new Result;
        p->word = t->word;
        p->file = t->file;
        p->content = t->content;
        p->line = t->line;
        p->PageRank = t->PageRank;
        t = t->next;
        p->next = NULL;

        if(DivResult1 == NULL ||(DivResult1 != NULL && DivResult1->word == p->word)){


            if(DivResult1 == NULL)
                DivResult1 = p;
            else{
                p->next = DivResult1;
                DivResult1 = p;
            }
        }
        else{
            if(DivResult2 == NULL)
                DivResult2 = p;
            else{
                p->next = DivResult2;
                DivResult2 = p;
            }
        }
    }

    bitSet<int> temp[1431];

    while(DivResult1){
        temp[DivResult1->file].addMember(DivResult1->line);
        DivResult1 = DivResult1->next;
    }
    for(int i = 0; i < 1431; ++i){
        if(type == AND_INIT || type == OR_INIT)
            final[i] = temp[i];

        else if(type == AND)
            final[i] * temp[i];

        else if(type == OR)
            final[i] + temp[i];
    }

    cout << endl;
    if(DivResult2 != NULL){
        if(type == AND_INIT || type == AND)
            DivideResult(DivResult2, AND);
        else
            DivideResult(DivResult2, OR);
    }

}



Result *BoolSearch(Result *head, int type){

    DivideResult(head, type);

    Result *newHead = NULL;
    for(int i = 0; i < 1431; ++i)
        for(int j = 1; j <= 1000; ++j){
            if(final[i].getMember(j) == 1){
                Result *t = new Result;
                t->file = i;
                t->line = j;

                t->next = NULL;

                if(newHead == NULL)
                    newHead = t;
                else{
                    t->next = newHead;
                    newHead = t;
                }
            }
        }
    for(Result *p = newHead; p != NULL; p = p->next)
        for(Result *q = head; q != NULL; q = q->next)
            if(q->line == p->line && q->file == p->file){
                p->word = q->word;
                p->content = q->content;
                p->PageRank = q->PageRank;
            }

    for(int i = 0; i < 1431; ++i)
        final[i].makeEmpty();
    return newHead;
}

