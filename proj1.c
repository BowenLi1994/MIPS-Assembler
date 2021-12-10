#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//Varible table
struct Table{

    char * varible;
    int address;
};

//allocate address to each line and each varible
void allocate_memory_address(char *line,int line_number, int * current_address, int  line_address[], struct Table * table, int * varible_number);
//Judge varible;
int isVarible(char * token);
//remove all spacing,enter,tab
char* cleanstring(char * str);
//allocate machine code
void allocate_machine_code(char *line,int line_number,int * line_address,struct Table table[],int varible_no);
//Judge instructions
int isInstruction(char * token);
//Instruction->Machine code
void instructions_transfer(char * instrction, char *oprand,int * line_address,int line_number,struct Table * table,int varible_no);
//transfer register to related number
int register_number(char * reg);
//Lookup the table
int lookuptable(struct Table * table,int varible_number,char * token);
//get bites between a to b
unsigned createMask(unsigned a, unsigned b);

//Main function
int main(){
    
    
    
    
    // varible: read file line by line
    char line[100][256];
    int line_number=0;
    //varible: line address
    int address_line[200];
    int i;
    for(i=0;i<200;i++)
        address_line[i]=-1;
    int current_address=0;
    //varible:  varible address
    struct Table varible_table[100];
    int varible_no=0;
    
    
    while(fgets(line[line_number], 256,stdin)!=NULL){
         
      
     //allocate address to each line and each varible
        allocate_memory_address(line[line_number],line_number,&current_address,address_line,varible_table,&varible_no);
        
      line_number++;
     }
//    for(int i=0;i<line_number;i++)
//        printf("line %d: %d\n",i+1,address_line[i]);
//    for(int i=0;i<varible_no;i++)
//        printf("varible:%s-->%d\n",varible_table[i].varible,varible_table[i].address);
//
    for(i=0;i<line_number;i++){
    // allocate address to each line of the mips
        allocate_machine_code(line[i],i,address_line,varible_table,varible_no);
    }
    
    return 0;
    
}

void allocate_memory_address(char *line,int line_number, int * current_address, int  line_address[], struct Table * table, int * varible_number){
    
    //split line
    char * line_dup, *found;
    line_dup=strdup(line);
    char*  line_content[3];
    int i;
    for(i=0;i<3;i++){
         line_content[i]=strdup("NULL");
        //printf("line_content: %s\n",line_content[i]);
    }
    int part_no=0;
    //split lines
    while((found=strsep(&line_dup,"\t"))!=NULL){
        if(strcmp(found,"")!=0&&strcmp(found," ")!=0){
            found=cleanstring(found);
            //printf("found %d: %s\n",part_no,found);
            //line_content[part_no] = (char*)malloc(strlen(found));
            //memcpy(line_content[part_no], found, strlen(found));
            line_content[part_no]=strdup(found);
            //printf("line_content %d: %s\n",part_no,line_content[part_no]);
            part_no++;
                }
        }
    //assign current address to the line_address array
    if(part_no!=1){
    line_address[line_number]=*current_address;
    }
    //current address change:
    //speacial case1: la

   if(strcmp(line_content[0], "la")==0||strcmp(line_content[1], "la")==0||strcmp(line_content[2], "la")==0){
       //printf("la:%d!!!!\n",line_number);
       *current_address=*current_address+8;
   }
   //speacial cse2: .space
   else if(strcmp(line_content[0], ".space")==0){
       *current_address=*current_address+atoi(line_content[1]);
   }
   else if(strcmp(line_content[1], ".space")==0){
       //printf(".space number: %d\n",atoi(line_content[2]));
        *current_address=*current_address+atoi(line_content[2]);
    }
    //.data/.text cases
    else if(part_no==1)
        *current_address=*current_address;
    else
        *current_address=*current_address+4;
        
    //There is varible
    if(isVarible(line_content[0])==1){
        char * temp=strdup(line_content[0]);
        temp[strlen(temp)-1]='\0';
        table[*varible_number].varible=strdup(temp);
        table[*varible_number].address=line_address[line_number];

        (*varible_number)++;
    }

    
    
    
    
}


int isVarible(char * token){
    
    char * pch;
    pch=strchr(token,':');
    if (pch!=NULL)
       return 1;
    else
       return 0;
}

int isInstruction(char * token){
    
   if(strcmp(token,"add")==0||strcmp(token,"addi")==0||strcmp(token,"nor")==0||strcmp(token,"ori")==0||strcmp(token,"sll")==0||strcmp(token,"lui")==0||strcmp(token,"sw")==0||strcmp(token,"lw")==0||strcmp(token,"bne")==0||strcmp(token,"j")==0||strcmp(token,"la")==0)
       return 1;
    else
        return 0;
}



char* cleanstring(char * str){
   //printf("str:%s",str);
    char  * result=(char*)malloc(strlen(str));
    int i;
    for(i=0;i<strlen(str);i++){
        if(str[i]!=' '&&str[i]!='\n'&&str[i]!='\t'&&str[i]!='\r'){
            //printf("%c\n",str[i]);
            strncat(result, &str[i],1);
        }

    }
    
    return result;
}

void allocate_machine_code(char *line,int line_number,int * line_address,struct Table table[],int varible_no){
    
    char * line_dup, *found;
    line_dup=strdup(line);
    char*  line_content[3];
    int i;
    for(i=0;i<3;i++)
        line_content[i]=strdup("NULL");
    int part_no=0;
       //split lines
    while((found=strsep(&line_dup,"\t"))!=NULL){
        if(strcmp(found,"")!=0){
            found=cleanstring(found);
            line_content[part_no]=strdup(found);
            //printf("line_content %d: %s\n",part_no,line_content[part_no]);
            part_no++;
                }
           }
    
    //.text
     if(part_no==1&&strcmp(line_content[0], ".text")){
         //printf("text section!\n");
         return;
     }
    //.data
    else if(part_no==1&&strcmp(line_content[0], ".data")){
        //printf("data section!\n");
        return;
    }
    //instruction   oprand
    else if(isInstruction(line_content[0])==1){
        //printf("Found instrcution: %s\n", line_content[0]);
        instructions_transfer(line_content[0], line_content[1], line_address, line_number, table, varible_no);
    }
    // varible instruction oprand
    else if(isInstruction(line_content[1])==1){
        //printf("Found instrcution: %s\n", line_content[1]);
        instructions_transfer(line_content[1], line_content[2], line_address, line_number, table, varible_no);
    }
}

void instructions_transfer(char * instrction, char *oprand,int * line_address,int line_number,struct Table * table,int varible_no){
     
    char * token[3];
    char * op_dup, *found;
    op_dup=strdup(oprand);
    int token_no=0;
    //split opcode
    while((found=strsep(&op_dup,","))!=NULL){
         token[token_no]=strdup(found);
         token_no++;
       }
    
    if(strcmp(instrction,"add")==0){
        int op_number=0;
        int rs=register_number(token[1]);
        int rt=register_number(token[2]);
        int rd=register_number(token[0]);
        int shamt=0;
        int funct=32;
        int32_t whole_ins = (op_number << 26) | (rs << 21) | (rt << 16) | (rd << 11) |(shamt << 6) | funct;
        
        printf("0x%0.8X ",line_address[line_number]);
        printf("0x%0.8X\n",whole_ins);
    }
    
    if(strcmp(instrction,"addi")==0){
       int op_no=8;
       int rs=register_number(token[1]);
       int rt=register_number(token[0]);
       int imm=atoi(token[2]);
       imm=imm&65535;
       int32_t whole_ins = (op_no << 26) | (rs << 21) | (rt << 16) | imm;
       printf("0x%0.8X ",line_address[line_number]);
       printf("0x%0.8X\n",whole_ins);
    }
    
    if(strcmp(instrction,"nor")==0){
       int op_number=0;
       int rs=register_number(token[1]);
       int rt=register_number(token[2]);
       int rd=register_number(token[0]);
       int shamt=0;
       int funct=39;
       int32_t whole_ins = (op_number << 26) | (rs << 21) | (rt << 16) | (rd << 11) |(shamt << 6) | funct;
       printf("0x%0.8X ",line_address[line_number]);
       printf("0x%0.8X\n",whole_ins);
    }
    
    if(strcmp(instrction,"ori")==0){
    int op_no=13;
    int rs=register_number(token[1]);
    int rt=register_number(token[0]);
    unsigned imm = atoi(token[2]);
    imm=imm&65535;
    int32_t whole_ins = (op_no << 26) | (rs << 21) | (rt << 16) | imm;
    printf("0x%0.8X ",line_address[line_number]);
    printf("0x%0.8X\n",whole_ins);
    }
    
    if(strcmp(instrction,"sll")==0){
        int op_no=0;
        int rs=0;
        int rt=register_number(token[1]);
        int rd=register_number(token[0]);
        int shamt=atoi(token[2]);
        int funct=0;
        int32_t whole_ins = (op_no << 26) | (rs << 21) | (rt << 16) | (rd << 11) |(shamt << 6) | funct;
        printf("0x%0.8X ",line_address[line_number]);
        printf("0x%0.8X\n",whole_ins);
    }
    
    if(strcmp(instrction,"lui")==0){
        int op_no=15;
        int rs=0;
        int rt=register_number(token[0]);
        int imm = atoi(token[1]);
        imm=imm&65535;
        int32_t whole_ins = (op_no << 26) | (rs << 21) | (rt << 16) | imm;
        printf("0x%0.8X ",line_address[line_number]);
        printf("0x%0.8X\n",whole_ins);
    }
    
    if(strcmp(instrction,"sw")==0){
              int op_no=43;
              int rt=register_number(token[0]);
              //printf("lw token[1]:%s\n",token[1] );
              char * token_dup, *found;
              token_dup=strdup(token[1]);
              char * offset_content[3];
              int token_no=0;
              //split lines
              while((found=strsep(&token_dup,"()"))!=NULL){
                  offset_content[token_no]=strdup(found);
                  token_no++;
               }
              int imm=atoi(offset_content[0]);
              imm=imm&65525;
              int rs=register_number(offset_content[1]);
              int32_t whole_ins = (op_no << 26) | (rs << 21) | (rt << 16) | imm;
             
              printf("0x%0.8X ",line_address[line_number]);
              printf("0x%0.8X\n",whole_ins);
    }

    if(strcmp(instrction,"lw")==0){
          int op_no=35;
          int rt=register_number(token[0]);
          //printf("lw token[1]:%s\n",token[1] );
          
          char * token_dup, *found;
          token_dup=strdup(token[1]);
          char * offset_content[3];
          int token_no=0;
          //split lines
          while((found=strsep(&token_dup,"()"))!=NULL){
              offset_content[token_no]=strdup(found);
              token_no++;
              }
         int imm=atoi(offset_content[0]);
         imm=imm&65535;
         int rs=register_number(offset_content[1]);
         int32_t whole_ins = (op_no << 26) | (rs << 21) | (rt << 16) | imm;
        
         printf("0x%0.8X ",line_address[line_number]);
         printf("0x%0.8X\n",whole_ins);
    }
    
      if(strcmp(instrction,"bne")==0){
                int op_no=5;
                int rs=register_number(token[0]);
                int rt=register_number(token[1]);
                int target=lookuptable(table, varible_no, token[2]);
                int target_address=(table[target].address-line_address[line_number]-4)/4;
               if(target_address<0)
                   target_address=target_address&65535;
                int32_t whole_ins = (op_no << 26) | (rs << 21) | (rt << 16) | target_address;

                printf("0x%0.8X ",line_address[line_number]);
                printf("0x%0.8X\n",whole_ins);
      }

    
      if(strcmp(instrction,"j")==0){
                int op_no=2;
                int target=lookuptable(table, varible_no, token[0]);
                int target_address=table[target].address/4;
          
                int32_t whole_ins = (op_no << 26) |target_address;
          
          printf("0x%0.8X ",line_address[line_number]);
          printf("0x%0.8X\n",whole_ins);
          
      }
    
    
    if(strcmp(instrction,"la")==0){
        
      token[1]=cleanstring(token[1]);
      //printf("token[1]:%s\n",token[1]);
      int target=lookuptable(table, varible_no, token[1]);
       
      //printf("target: %d\n",target);
       
      int lable_address=table[target].address;
      //printf("la->label address: %d\n",lable_address);
      //lui
      int op_no1=15;
      int rs1=0;
      int rt1=register_number("$1");
      //int r = createMask(17,32);
      //unsigned imm1 = r & lable_address;

       int imm1=lable_address>>16;
       //printf("la->lui: %d\n",imm1);
        imm1=imm1&65535;
      int32_t whole_ins1 = (op_no1 << 26) | (rs1 << 21) | (rt1 << 16) | imm1;
      printf("0x%0.8X ",line_address[line_number]);
      printf("0x%0.8X\n",whole_ins1);
        
        
      //ori
      int op_no2=13;
      int rs2=register_number("$1");
      int rt2=register_number(token[0]);
      //r = createMask(0,15);
      //unsigned imm2 = r & lable_address;
      int imm2=lable_address&65535;
      int32_t whole_ins2 = (op_no2 << 26) | (rs2 << 21) | (rt2 << 16) | imm2;
      printf("0x%0.8X ",line_address[line_number]+4);
      printf("0x%0.8X\n",whole_ins2);
       //array 前半段
        
       
        
    }
    
    
}

int register_number(char * reg){
    
    if(strcmp(reg,"$0")==0)
        return 0;
    if(strcmp(reg,"$1")==0)
        return 1;
    if(reg[1]=='t'){
        
        return 8+(reg[2]-'0');
    }
    else if(reg[1]=='s'){
        
        return 16+(reg[2]-'0');
    }
    return -1;
}

int lookuptable(struct Table * table,int varible_number,char * token){
    
    
    
    int flag=-1;
    int i;
    for(i=0;i<varible_number;i++){
        table[i].varible=cleanstring(table[i].varible);
        if(strcmp(table[i].varible,token)==0)
            flag=i;
    }
    return flag;
}

//get the bits between a to b;
unsigned createMask(unsigned a, unsigned b)
{
   unsigned r = 0;
    unsigned i;
   for (i=a; i<=b; i++)
       r |= 1 << i;

   return r;
}
