#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */
#include <string.h> /* strcmp() strlen() */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}
static int lept_parse_single_type(lept_context* c, lept_value* v, const char *s){
    EXPECT(c, s[0]);
    for(int i=1;s[i]!='\0';++i){
        if(c->json[i - 1] != s[i])
            return LEPT_PARSE_INVALID_VALUE;
    }
    c->json += strlen(s) - 1;
    if(!strcmp(s, "null")){
        v->type = LEPT_NULL;
    }else if(!strcmp(s, "true")){
        v->type = LEPT_TRUE;        
    }else{
        v->type = LEPT_FALSE;        
    }
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 'n':  return lept_parse_single_type(c, v, "null");
        case 't':  return lept_parse_single_type(c, v, "true");
        case 'f':  return lept_parse_single_type(c, v, "false");
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    int ret = lept_parse_value(&c, v);
    if (ret == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0')
            return LEPT_PARSE_ROOT_NOT_SINGULAR;
        else
            return LEPT_PARSE_OK;
    } else {
        return ret;
    }
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}
