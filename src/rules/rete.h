#include "state.h"

#define OP_NOP 0
#define OP_LT 0x01
#define OP_LTE 0x02
#define OP_GT 0x03
#define OP_GTE 0x04
#define OP_EQ 0x05
#define OP_NEQ 0x06
#define OP_EX 0x07
#define OP_NEX 0x08
#define OP_ALL 0x09
#define OP_ANY 0x0A
#define OP_OR 0x0B
#define OP_AND 0x0C
#define OP_END 0x0D
#define OP_ADD 0x0E
#define OP_SUB 0x0F
#define OP_MUL 0x10
#define OP_DIV 0x11
#define OP_TYPE 0x12
#define OP_NOT 0x13
#define OP_MT 0x14
#define OP_IMT 0x15
#define OP_IALL 0x16
#define OP_IANY 0x17

#define NODE_ALPHA 0
#define NODE_BETA_CONNECTOR 1
#define NODE_ACTION 2
#define NODE_M_OFFSET 0

#define MAX_STATE_INDEX_LENGTH 1024
#define MAX_LEFT_FRAME_INDEX_LENGTH 1024
#define MAX_RIGHT_FRAME_INDEX_LENGTH 128
#define MAX_EXPRESSION_TERMS 32

typedef struct reference {
    unsigned int nameHash;
    unsigned int nameOffset;
    unsigned int idOffset;
} reference;

typedef struct regexReference {
    unsigned int stringOffset;
    unsigned int stateMachineOffset;
    unsigned short statesLength;
    unsigned short vocabularyLength;
} regexReference;

typedef struct jsonValue {
    unsigned char type;
    union { 
        long i; 
        double d; 
        unsigned char b; 
        unsigned int stringOffset;
        unsigned int idiomOffset;
        regexReference regex;
        reference property;
    } value;
} jsonValue;

typedef struct idiom {
    unsigned char operator;
    jsonValue left;
    jsonValue right; 
} idiom;

typedef struct expression {
    unsigned int nameOffset;
    unsigned int aliasOffset;
    unsigned short termsLength;
    unsigned char distinct;
    unsigned char not;
    unsigned int terms[MAX_EXPRESSION_TERMS];
} expression;

typedef struct alpha {
    unsigned int hash;
    unsigned char operator;
    unsigned int betaListOffset;
    unsigned int nextListOffset;
    unsigned int nextOffset;
    jsonValue right;
} alpha;

typedef struct beta {
    unsigned int leftFrameIndex[MAX_LEFT_FRAME_INDEX_LENGTH];
    unsigned int rightFrameIndex[MAX_RIGHT_FRAME_INDEX_LENGTH];
    unsigned int expressionOffset;
    unsigned int hash;
    unsigned int nextOffset;
    unsigned char not;
} beta;

typedef struct action {
    unsigned int index;
    unsigned short count;
    unsigned short cap;
    unsigned short priority;
    unsigned int joinsOffset;
    unsigned short joinsLength;
} action;

typedef struct node {
    unsigned int nameOffset;
    unsigned char type;
    union { 
        alpha a; 
        beta b; 
        action c; 
    } value;
} node;

typedef struct ruleset {
    unsigned int nameOffset;
    unsigned int actionCount;
    void *bindingsList;
    
    node *nodePool;
    unsigned int nodeOffset;
    
    unsigned int *nextPool;
    unsigned int nextOffset;
    
    char *stringPool;
    unsigned int stringPoolLength; 
    
    expression *expressionPool;
    unsigned int expressionOffset;
    
    idiom *idiomPool;
    unsigned int idiomOffset;
    
    char *regexStateMachinePool;
    unsigned int regexStateMachineOffset;
    
    pool statePool;
    unsigned int stateIndex[MAX_STATE_INDEX_LENGTH];
    
    pool messagePool;

    pool leftFramePool;

    pool rightFramePool;

    unsigned int orNodeOffset;
    unsigned int andNodeOffset;
    unsigned int endNodeOffset;
} ruleset;


