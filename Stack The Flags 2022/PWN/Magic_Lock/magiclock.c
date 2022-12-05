
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "def.h"

// Global var
int CurrentSpellType = 1;
void* Spell;

// Declare functions
int configure_spell();
int cast_spell();
int calc_recv();

int start()
{
    char buffer[10];
    int option = 1;
    
    // Inti msg strings
    char menu_msg[] = ""
    "==========================\n"
    "You are trapped in a cage in a dark cave\n"
    "The cage is sealed by a magic lock\n"
    "What do you do?\n"
    "==========================\n\n"
    "1)Attempt to pick the lock\n\n"
    "2)Cast Spell\n\n"
    "3)Review your Spell Book\n\n"
    "4)Yell for HELP!\n\n";

    char option1[] = ""
    "==========================\n"
    "You desperately fiddle with the lock using your lock pick.\n"
    "The lockpick breaks\n"
    "You grunt.\n"
    "You convince yourself lockpicking is too hard because you're a model citizen \n"
    "Thankfully you still have extra, try again?\n"
    "==========================\n\n";
   
    char option4[] = ""
    "==========================\n"
    "\"HELPPP!! HEEELP!!\' -You yell\n"
    "Your screams echo back at you\n"
    ":(\n"
    "Scream for help again?\n"
    "==========================\n\n";

     char castmsg[] = ""
    "\n=============================\n"
    "You close your eyes to conjur the mana within...\n"
    "==============================\n\n"; 

     char castfailmsg[] = ""
    "\n=============================\n"
    "You blast the lock with all your might\n"
    "The magic lock remains unscathed.\n"
    "\"NEED MORE DMG!!!\" -You scream\n"
    "==============================\n\n"; 

    // Init spell
    Spell= (void*)malloc(sizeof(Telekenisis));

    // Init Fireball default spell
    strcpy(Spell+4, "Fireball");
    *(unsigned short*)Spell = 50;
    *(short*)(Spell+2) = 50;
    
    CurrentSpellType = 1;

    // Get user menu input
    while(1)
    {
        // Print lock menu
        printf(menu_msg);

        // Read userinput
        scanf("%1s", buffer);
        option = atoi(buffer);

        // Choose what to do
        if (option==1)
        {
            printf(option1);
        }
        else if (option==2)
        {
            // Cast msg
            printf( castmsg); 

            // Calc receive len
            int recv_len = calc_recv();

            if (recv_len == 1)
            {
                continue;
            }

            // Cast spell
            cast_spell(recv_len);

            // Cast fail msg
            printf( castfailmsg); 
        }
        else if (option==3)
        {
            configure_spell();
        }
        else if (option==4)
        {
            printf(option4);
        }
        
    }

    // Cleanup
    free(Spell);
    return 0;

}

int configure_spell()
{
    char buffer[10];
    int option;
    int userinput;

    // Get user menu input
    char spellbookmsg[] = ""
    "==========================\n"
    "You open your spellbook\n"
    "There is a variety of spells in it\n"
    "You decide to use your signature spells\n\n"
    "Choose a Spell!\n"
    "==========================\n\n"
    "1)Fireball\n\n"
    "2)Telekenisis\n\n";

    printf(spellbookmsg);

    // Read userinput
    scanf("%1s", buffer);
    option = atoi(buffer);
    
    // Fireball
    if (option == 1)
    {
        char errmsg[] = ""
        "\n==========================\n"
        "Invalid Input. The magic fizzles\n"
        "==========================\n\n";

        strcpy((Spell+4), "Fireball");
        CurrentSpellType = 1;

        // Set size
        printf("What is the size of the fireball? (1-65535)");
        scanf("%5s", buffer);
        userinput = atoi(buffer);

        if (userinput >=1 && userinput <=65535)
        {
            *(short*)(Spell) = (short)userinput;
        }
        else
        {
            printf(errmsg);
            return 1;
        }
        
        // Set dmg
        printf("How much damage does the fireball do? (1-100)");      
        scanf("%3s", buffer);
        userinput = atoi(buffer);

        if (userinput >=1 && userinput <=100)
        {
            *(short*)(Spell+2) = (short)userinput;
        }
        else
        {
            printf(errmsg);
            return 1;
        }
        
    }


    // Telekenisis
    if (option == 2)
    {
        char errmsg[] = ""
        "\n==========================\n"
        "Invalid Input. The magic fizzles\n"
        "==========================\n\n";

        strcpy((Spell+4), "Telekenisis");
        CurrentSpellType = 2;
        
        // Set dmg
        printf("How much damage does the telekenisis do? (1-100)");        
        scanf("%3s", buffer);
        userinput = atoi(buffer);

        if (userinput >=1 && userinput <=100)
        {
            *(short*)(Spell) = (short)userinput;
        }
        else
        {
            printf(errmsg);
            return 1;
        }

        // Set dist
        printf("What is the distance of the telekenisis? (1-100)");
        scanf("%3s", buffer);
        userinput = atoi(buffer);

        if (userinput >=1 && userinput <=100)
        {
            *(short*)(Spell+2) = (short)userinput;
        }
        else
        {
            printf(errmsg);
            return 1;
        }
    }


    printf("\n==========================\n\n");
    return 0;
}

int calc_recv()
{
    int recv_len =30;

    // Messages
    char errmsg[] = ""
    "\n=============================\n"
    "Invalid Input. The magic fizzles\n"
    "\n==============================\n\n";

    char recvmsg[] = ""
    "\n=============================\n"
    "The air around you starts to vibrate.\n"
    "Now all thats left is to unleash the arcane!\n"
    "==============================\n\n";

    char recvmsg2[] = ""
    "\n=============================\n"
    "The air around you starts to vibrate.\n"
    "Now all thats left is to unleash the arcane!\n\n"
    "You've got a good feeling about this!\n"
    "==============================\n\n";

    // Calc Receive length based on spell type dmg
    if (CurrentSpellType == 1)
    {
        // Validate dmg
        if (((pFireball)Spell)->dmg >= 30)
        {
            printf(errmsg);
            return 1;
        }

        // Calc
        recv_len = recv_len - ((pFireball)Spell)->dmg;
        
    }
    else
    {
        // Validate dmg
        if (((pTelekenisis)Spell)->dmg >= 30)
        {
            printf(errmsg);
            return 1;
        }

        // Calc
        recv_len = recv_len - ((pTelekenisis)Spell)->dmg;
    }    

    // Check valid recv length
    if (recv_len <= 0)
    {
        printf(errmsg);
        return 1;
    }
    
    // Print validate succ message
    if (recv_len > 30)
    {
        printf(recvmsg2);
    }
    else
    {
        printf(recvmsg);
    }

    return recv_len; 
}

int cast_spell(int recv_len )
{
    // Read userinput
    char buffer[30];

    // Receive the spell
    getchar();
    fgets(buffer, recv_len, stdin);
    
    return 0;
}

int unlock_lock(unsigned int passcode1, long unsigned int passcode2, long unsigned int passcode3)
{
    FILE *fp;
    char *flag;
    long fsize;

    // Validate
    if (passcode1 == 0xfeedbeef && passcode2 != 0xdeadcafebabebeef && passcode3 != 0xdeadcafebabebeef)
    {
        // Send error
        printf( "Rogue Magic Disspelled! Invalid Spell Arg");
        return 1;
    }

    // Opening file for reading 
    fp = fopen("flag.txt" , "rb");
    if(fp == NULL) 
    {
      perror("Error opening file");
      return(-1);
    }
    
    // Read flag
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);  

    flag = (char*)malloc(fsize + 1);
    fread(flag, fsize, 1, fp);
    fclose(fp);

    *(flag+fsize) = 0;

    // Send back flag
    printf(flag);

    // Close file
    fclose(fp);   
    return 0;
}


int main(int argc, char **argv) 
{
    // Null stream buffers
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	setbuf(stderr, NULL);

    // Begin Challenge
    start();

    return 0;
}





