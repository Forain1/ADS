#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define TRUE 1
#define FALSE 0

int stem(char *p, int index, int position);

static char *b;

static int k;
static int k0;

/* j is a general offset into the string */
static int j;

/**
 * TRUE when `b[i]` is a consonant.
 */

static int
isConsonant(int index) {
  switch (b[index]) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      return FALSE;
    case 'y':
      return (index == k0) ? TRUE : !isConsonant(index - 1);
    default:
      return TRUE;
  }
}

/* Measure the number of consonant sequences between
 * `k0` and `j`.  If C is a consonant sequence and V
 * a vowel sequence, and <..> indicates arbitrary
 * presence:
 *
 *   <C><V>       gives 0
 *   <C>VC<V>     gives 1
 *   <C>VCVC<V>   gives 2
 *   <C>VCVCVC<V> gives 3
 *   ....
 */
static int
getMeasure() {
  int position;
  int index;

  position = 0;
  index = k0;

  while (TRUE) {
    if (index > j) {
      return position;
    }

    if (!isConsonant(index)) {
      break;
    }

    index++;
  }

  index++;

  while (TRUE) {
    while (TRUE) {
      if (index > j) {
        return position;
      }

      if (isConsonant(index)) {
        break;
      }

      index++;
    }

    index++;
    position++;

    while (TRUE) {
      if (index > j) {
        return position;
      }

      if (!isConsonant(index)) {
        break;
      }

      index++;
    }

    index++;
  }
}

/* `TRUE` when `k0, ... j` contains a vowel. */
static int
vowelInStem() {
  int index;

  index = k0 - 1;

  while (++index <= j) {
    if (!isConsonant(index)) {
      return TRUE;
    }
  }

  return FALSE;
}

/* `TRUE` when `j` and `(j-1)` are the same consonant. */
static int
isDoubleConsonant(int index) {
  if (b[index] != b[index - 1]) {
    return FALSE;
  }

  return isConsonant(index);
}

/* `TRUE` when `i - 2, i - 1, i` has the form
 * `consonant - vowel - consonant` and also if the second
 * C is not `"w"`, `"x"`, or `"y"`. this is used when
 * trying to restore an `e` at the end of a short word.
 *
 * Such as:
 *
 * `cav(e)`, `lov(e)`, `hop(e)`, `crim(e)`, but `snow`,
 * `box`, `tray`.
 */
static int
cvc(int index) {
  int character;

  if (index < k0 + 2 || !isConsonant(index) || isConsonant(index - 1) || !isConsonant(index - 2)) {
    return FALSE;
  }

  character = b[index];

  if (character == 'w' || character == 'x' || character == 'y') {
    return FALSE;
  }

  return TRUE;
}

/* `ends(s)` is `TRUE` when `k0, ...k` ends with `value`. */
static int
ends(const char *value) {
  int length = value[0];

  /* Tiny speed-up. */
  if (value[length] != b[k]) {
    return FALSE;
  }

  if (length > k - k0 + 1) {
    return FALSE;
  }

  if (memcmp(b + k - length + 1, value + 1, length) != 0) {
    return FALSE;
  }

  j = k - length;

  return TRUE;
}

/* `setTo(value)` sets `(j + 1), ...k` to the characters in
 * `value`, readjusting `k`. */
static void
setTo(const char *value) {
  int length = value[0];

  memmove(b + j + 1, value + 1, length);

  k = j + length;
}

/* Set string. */
static void
replace(const char *value) {
  if (getMeasure() > 0) {
    setTo(value);
  }
}

/* `step1ab()` gets rid of plurals, `-ed`, `-ing`.
 *
 * Such as:
 *
 *   caresses  ->  caress
 *   ponies    ->  poni
 *   ties      ->  ti
 *   caress    ->  caress
 *   cats      ->  cat
 *
 *   feed      ->  feed
 *   agreed    ->  agree
 *   disabled  ->  disable
 *
 *   matting   ->  mat
 *   mating    ->  mate
 *   meeting   ->  meet
 *   milling   ->  mill
 *   messing   ->  mess
 *
 *   meetings  ->  meet
 */
static void
step1ab() {
  int character;

  if (b[k] == 's') {
    if (ends("\04" "sses")) {
      k -= 2;
    } else if (ends("\03" "ies")) {
      setTo("\01" "i");
    } else if (b[k - 1] != 's') {
      k--;
    }
  }

  if (ends("\03" "eed")) {
    if (getMeasure() > 0) {
      k--;
    }
  } else if ((ends("\02" "ed") || ends("\03" "ing")) && vowelInStem()) {
    k = j;

    if (ends("\02" "at")) {
      setTo("\03" "ate");
    } else if (ends("\02" "bl")) {
      setTo("\03" "ble");
    } else if (ends("\02" "iz")) {
      setTo("\03" "ize");
    } else if (isDoubleConsonant(k)) {
      k--;

      character = b[k];

      if (character == 'l' || character == 's' || character == 'z') {
        k++;
      }
    } else if (getMeasure() == 1 && cvc(k)) {
      setTo("\01" "e");
    }
  }
}

/* `step1c()` turns terminal `"y"` to `"i"` when there
 * is another vowel in the stem. */
static void
step1c() {
  if (ends("\01" "y") && vowelInStem()) {
    b[k] = 'i';
  }
}

/* `step2()` maps double suffices to single ones.
 * so -ization ( = -ize plus -ation) maps to -ize etc.
 * note that the string before the suffix must give
 * getMeasure() > 0. */
static void
step2() {
  switch (b[k - 1]) {
    case 'a':
      if (ends("\07" "ational")) {
        replace("\03" "ate");
        break;
      }

      if (ends("\06" "tional")) {
        replace("\04" "tion");
        break;
      }

      break;
    case 'c':
      if (ends("\04" "enci")) {
        replace("\04" "ence");
        break;
      }

      if (ends("\04" "anci")) {
        replace("\04" "ance");
        break;
      }

      break;
    case 'e':
      if (ends("\04" "izer")) {
        replace("\03" "ize");
        break;
      }

      break;
    case 'l':
      /* --DEPARTURE--: To match the published algorithm,
       * replace this line with:
       *
       * ```
       * if (ends("\04" "abli")) {
       *     replace("\04" "able");
       *
       *     break;
       * }
       * ```
       */
      if (ends("\03" "bli")) {
        replace("\03" "ble");
        break;
      }

      if (ends("\04" "alli")) {
        replace("\02" "al");
        break;
      }

      if (ends("\05" "entli")) {
        replace("\03" "ent");
        break;
      }

      if (ends("\03" "eli")) {
        replace("\01" "e");
        break;
      }

      if (ends("\05" "ousli")) {
        replace("\03" "ous");
        break;
      }

      break;
    case 'o':
      if (ends("\07" "ization")) {
        replace("\03" "ize");
        break;
      }

      if (ends("\05" "ation")) {
        replace("\03" "ate");
        break;
      }

      if (ends("\04" "ator")) {
        replace("\03" "ate");
        break;
      }

      break;
    case 's':
      if (ends("\05" "alism")) {
        replace("\02" "al");
        break;
      }

      if (ends("\07" "iveness")) {
        replace("\03" "ive");
        break;
      }

      if (ends("\07" "fulness")) {
        replace("\03" "ful");
        break;
      }

      if (ends("\07" "ousness")) {
        replace("\03" "ous");
        break;
      }

      break;
    case 't':
      if (ends("\05" "aliti")) {
        replace("\02" "al");
        break;
      }

      if (ends("\05" "iviti")) {
        replace("\03" "ive");
        break;
      }

      if (ends("\06" "biliti")) {
        replace("\03" "ble");
        break;
      }

      break;
    /* --DEPARTURE--: To match the published algorithm, delete this line. */
    case 'g':
      if (ends("\04" "logi")) {
        replace("\03" "log");
        break;
      }
  }
}

/* `step3()` deals with -ic-, -full, -ness etc.
 * similar strategy to step2. */
static void
step3() {
  switch (b[k]) {
    case 'e':
      if (ends("\05" "icate")) {
        replace("\02" "ic");
        break;
      }

      if (ends("\05" "ative")) {
        replace("\00" "");
        break;
      }

      if (ends("\05" "alize")) {
        replace("\02" "al");
        break;
      }

      break;
    case 'i':
      if (ends("\05" "iciti")) {
        replace("\02" "ic");
        break;
      }

      break;
    case 'l':
      if (ends("\04" "ical")) {
        replace("\02" "ic");
        break;
      }

      if (ends("\03" "ful")) {
        replace("\00" "");
        break;
      }

      break;
    case 's':
      if (ends("\04" "ness")) {
        replace("\00" "");
        break;
      }

      break;
  }
}

/* `step4()` takes off -ant, -ence etc., in
 * context <c>vcvc<v>. */
static void
step4() {
  switch (b[k - 1]) {
    case 'a':
      if (ends("\02" "al")) {
        break;
      }

      return;
    case 'c':
      if (ends("\04" "ance")) {
        break;
      }

      if (ends("\04" "ence")) {
        break;
      }

      return;
    case 'e':
      if (ends("\02" "er")) {
        break;
      }

      return;
    case 'i':
      if (ends("\02" "ic")) {
        break;
      }

      return;
    case 'l':
      if (ends("\04" "able")) {
        break;
      }

      if (ends("\04" "ible")) {
        break;
      }

      return;
    case 'n':
      if (ends("\03" "ant")) {
        break;
      }

      if (ends("\05" "ement")) {
        break;
      }

      if (ends("\04" "ment")) {
        break;
      }

      if (ends("\03" "ent")) {
        break;
      }

      return;
    case 'o':
      if (ends("\03" "ion") && j >= k0 && (b[j] == 's' || b[j] == 't')) {
        break;
      }

      /* takes care of -ous */
      if (ends("\02" "ou")) {
        break;
      }

      return;
    case 's':
      if (ends("\03" "ism")) {
        break;
      }

      return;
    case 't':
      if (ends("\03" "ate")) {
        break;
      }

      if (ends("\03" "iti")) {
        break;
      }

      return;
    case 'u':
      if (ends("\03" "ous")) {
        break;
      }

      return;
    case 'v':
      if (ends("\03" "ive")) {
        break;
      }

      return;
    case 'z':
      if (ends("\03" "ize")) {
        break;
      }

      return;
    default:
      return;
  }

  if (getMeasure() > 1) {
    k = j;
  }
}

/* `step5()` removes a final `-e` if `getMeasure()` is
 * greater than `1`, and changes `-ll` to `-l` if
 * `getMeasure()` is greater than `1`. */
static void
step5() {
  int a;

  j = k;

  if (b[k] == 'e') {
    a = getMeasure();

    if (a > 1 || (a == 1 && !cvc(k - 1))) {
      k--;
    }
  }

  if (b[k] == 'l' && isDoubleConsonant(k) && getMeasure() > 1) {
    k--;
  }
}

/* In `stem(p, i, j)`, `p` is a `char` pointer, and the
 * string to be stemmed is from `p[i]` to
 * `p[j]` (inclusive).
 *
 * Typically, `i` is zero and `j` is the offset to the
 * last character of a string, `(p[j + 1] == '\0')`.
 * The stemmer adjusts the characters `p[i]` ... `p[j]`
 * and returns the new end-point of the string, `k`.
 *
 * Stemming never increases word length, so `i <= k <= j`.
 *
 * To turn the stemmer into a module, declare 'stem' as
 * extern, and delete the remainder of this file. */
int
stem(char *p, int index, int position) {
  /* Copy the parameters into statics. */
  b = p;
  k = position;
  k0 = index;

  // if (k <= k0 + 1) {
  //   return k; /* --DEPARTURE-- */
  // }

  /* With this line, strings of length 1 or 2 don't
   * go through the stemming process, although no
   * mention is made of this in the published
   * algorithm. Remove the line to match the published
   * algorithm. */
  step1ab();

  if (k > k0) {
    step1c();
    step2();
    step3();
    step4();
    step5();
  }

  return k;
}



typedef struct postNode{
    int num;
    int document;
    struct postNode* nextPost;
}postNode;


typedef postNode* pPostNode;

typedef struct termNode{
    char word [21];
    int times;
    struct termNode* nextTerm;
    pPostNode nextPost;
}termNode;

typedef termNode* pTermNode;



pTermNode readDoc(char** documents,int n,pTermNode root,int* wordnum){//还有过滤词汇的模块没有完成
        int i;
        pTermNode tempnode;
       
        for(i=0;i<n;i++){
        //处理多余的回车键
        char* document = (char*)malloc(7*sizeof(char));
        scanf("%s\n",document);
        //char tempchar;
        //对n个文档分别处理
        documents[i]=document;
        
        //test
        // printf("%s\n%s",document,documents[i]);
        //test
        
        char word [21];
        // int j=0;
        while(1){
            scanf("%s",word);

            if(word[0]=='#')break;


            //int k=0;
            // char banchr [5];
            // int  banpoint=0 ;
            // int banint=0;
            // int lastbanint=0;
            // for(k=0;k<strlen(word);k++){
            //     if(word[k]>='A'&&word[k]<='Z')word[k]=word[k]-'A'+'a';
            //     // banint = k;
            //     // if(word[k]=='e'||word[k]=='i'){
            //     //     banchr[banpoint++]=word[k];
            //     //     lastbanint = banint;
            //     // }
            //     // if(word[k]=='s'||word[k]=='d'){
            //     //     if(lastbanint)
            //     // }
            // }
            // int end;
            // if(strlen(word)>2){
            //   end = stem(word,0,strlen(word)-1);
            //   word[end+1]='\0';
            // }else{
            //   for(int x=0;x<strlen(word);x++){
            //     if(word[x])
            //   }
            // }
            for(int z = 0;z<strlen(word);z++){
              if(word[z]>='A'&&word[z]<='Z')word[z]=word[z]-'A'+'a';
            }
            

              int end =stem(word,0,strlen(word)-1);
              word[end+1]=0;

            

            
            // for(k=strlen(word)-1;k>=0;k--){

            // }

            
            //对每个词的处理
             tempnode=root;
            // j=0;
            // if(tempchar!='\n'&&tempchar!=' '){
            //     word[j++]=tempchar;
            // }else continue;

            // while((tempchar=getchar())!=' '&&tempchar!='\n'){
            //     word[j++]=tempchar;
            //     if(j>19)break;
            // }
            // word[j++]='\0';
            


            //根节点为空的情况,即最开始的初始化
            if(tempnode==NULL){
                //Term根节点的初始化
                root = (pTermNode)malloc(sizeof(termNode));
                strcpy(root->word,word);
                root->times=1;
                pPostNode rootPost = (pPostNode)malloc(sizeof(postNode));
                root->nextPost=rootPost;
                root->nextTerm=NULL;


                //Post根节点的初始化
                rootPost->document=i;
                rootPost->nextPost=NULL;
                rootPost->num=1;
                
                *wordnum=1;
                continue;
            }
            
            //根节点不为空，这个时候需要在Term表中查找是否有这个词
            while(1){
                if(strcmp(word,tempnode->word)==0){
                    tempnode->times++;
                    pPostNode temppost=tempnode->nextPost;

                    while(temppost->document!=i&&temppost->nextPost!=NULL){
                        temppost=temppost->nextPost;
                    }
                    
                    if(temppost->document==i){
                        temppost->num++;
                    }else{
                        pPostNode newpost = (pPostNode)malloc(sizeof(postNode));
                        newpost->nextPost=NULL;
                        newpost->document=i;
                        newpost->num=1;
                        temppost->nextPost=newpost;
                    }
                    
                    break;
                }else if(tempnode->nextTerm!=NULL){
                    tempnode=tempnode->nextTerm;
                }else{
                    //新建Term节点并初始化
                    pTermNode newterm = (pTermNode)malloc(sizeof(termNode));
                    newterm->times=1;
                    strcpy(newterm->word,word);
                    pPostNode newpost =(pPostNode)malloc(sizeof(postNode));
                    newterm->nextPost=newpost;
                    newterm->nextTerm=NULL;

                    tempnode->nextTerm=newterm;
                    
                    //新建Post节点并初始化
                    newpost->document=i;
                    newpost->nextPost=NULL;
                    newpost->num=1;

                    (*wordnum)++;
                    break;

                }
            }
        }
    }

    return root;
}


// int findDoc(char** documents,int n,int* doc1,int* doc2){
    
//     char tempchar;
//     char doc1name [8];
//     char doc2name [8];
//     (*doc1)=(*doc2)=-1;
//     int i=0;
//     while((tempchar=getchar())!=' '){
//         doc1name[i]=tempchar;
//         i++;
//     }
//     doc1name[i]='\0';
//     i=0;
//     while((tempchar=getchar())!='\n'){
//         doc2name[i]=tempchar;
//         i++;
//     }
//     doc2name[i]='\0';

//     (*doc1)=(*doc2)=0;
//     // for(i=0;i<n;i++){
//     //     if(strcmp(doc1name,documents[i])==0)(*doc1)=i;
//     //     if(strcmp(doc2name,documents[i])==0)(*doc2)=i;
//     //     if((*doc1)!=-1&&(*doc2)!=-1)break;
//     // }

//     if((*doc1)!=-1&&(*doc2)!=-1)return 0;
//     else return 1 ;



// }


double calDisVal(pTermNode root,int doc1,int doc2,int n){


    int flag1,flag2;
    int index=0;
    int i;
    pTermNode term = root;
    pPostNode doc = NULL;
    int* array1=(int*)malloc(n*sizeof(int));
    int* array2=(int*)malloc(n*sizeof(int));
    while(term!=NULL){
        doc = term->nextPost;
        flag1=flag2=0;
        while(doc!=NULL&&(!flag1||!flag2)){
            if(doc->document==doc1){
                array1[index]=doc->num;
                flag1=1;
            }
            if(doc->document==doc2){
                array2[index]=doc->num;
                flag2=1;
            }
            doc=doc->nextPost;
        }
        if(!flag1)array1[index]=0;
        if(!flag2)array2[index]=0;
        index++;
        term=term->nextTerm;
    }

    double product1=0,product2=0,product12=0;

    for(i=0;i<n;i++){
        product12=product12+array1[i]*array2[i];
        product1=product1+array1[i]*array1[i];
        product2=product2+array2[i]*array2[i];
    }

    double distance_metric = product12/(sqrt(product1)*sqrt(product2));
    
    double angle_metirc = acos(distance_metric);

    free(array1);
    free(array2);

    return angle_metirc;



}


int main(){

    int i,n;
    int doc1=-1,doc2=-1;
    int wordnum=0;
    scanf("%d",&n);
    char** documents = (char**)malloc(n*sizeof(char*));
    int docnum = n;
    pTermNode root = NULL;
    //读取文件
    root = readDoc(documents,docnum,root,&wordnum);

    
 
    // printf("\n\n\n");
    // pTermNode temp=root;
    // pPostNode temppost=NULL;
    // while(temp!=NULL){
    //     temppost=temp->nextPost;
    //     printf("%s:",temp->word);
    //     while(temppost!=NULL){
    //         printf("|%d %d|",temppost->document,temppost->num);
    //         temppost=temppost->nextPost;
    //     }
    //     printf("\n");
    //     temp=temp->nextTerm;

    // }
    // printf("\n\n\n");
    
    // int test=0;

    scanf("%d\n",&n);
    for(i=0;i<n;i++){
    
    //找到文件对应的编号

    //doc1=doc2=0;

    doc1=doc2=-1;
    char tempchar;
    char doc1name [8];
    char doc2name [8];
    int j=0;

    scanf("%s",&doc1name);
    scanf("%s",&doc2name);

    //j会大于7？
    // while((tempchar=getchar())!=' '&&tempchar!='\n'){
    //     doc1name[j]=tempchar;
    //     j++;
    //     //if(j>7)break;
    // }
    // doc1name[j]='\0';
    //     j=0;
    // while((tempchar=getchar())!='\n'&&tempchar!=' '){
    //     doc2name[j]=tempchar;
    //     j++;
    //     //if(j>7)break;
    // }
    // doc2name[j]='\0';

        for(j=0;j<docnum;j++){
        if(strcmp(doc1name,documents[j])==0)doc1=j;
        if(strcmp(doc2name,documents[j])==0)doc2=j;

    }

    // if(test)break;


    //计算词汇重合度

    printf("Case %d: %.3f\n",i+1,calDisVal(root,doc1,doc2,wordnum));
    
    }
}




