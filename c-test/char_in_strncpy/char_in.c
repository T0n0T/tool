#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "IO_config.h"

#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])
static const struct pin_list list[]=
{
        IO0_0,
        IO0_1,
        IO0_2,
        IO0_3,
        IO0_4,
        IO0_5,
        IO0_6,
        IO0_7,

        IO1_0,
        IO1_1,
        IO1_2,
        IO1_3,
        IO1_4,
        IO1_5,
        IO1_6,
        IO1_7,

};

const struct pin_list *get_list(char *pin)
{
    const struct pin_list *index;
    index = NULL;
    for(uint8_t i=0; i<ITEM_NUM(list);i++)
    {
        if(strncmp(list[i].name, pin, sizeof(list[i].name))==0)
        {
            index = &list[i];
            printf("%s has been chosen.\n",index->name);
            printf("%d is the sizeof a pin name.\n",sizeof(list[i].name));
            goto __exit;
        }
    }

    __exit:
    if(index == NULL)
    {
        printf("%s is not a usable pin, check the char whether conform to format like IO1_1 ",pin);
    }

    return index;
}

int main()
{
    char *str ;
    str = "IO0_7";
    const struct pin_list *index;

    index = get_list(str);

    if(index != NULL)
    {
        printf("the pin we get is : %s\n",index->name);
        return 0;
    }        
    else
    {
        return 0;
    }
        
}