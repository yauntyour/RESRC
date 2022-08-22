#ifndef __COROUTINE__H__
#define __COROUTINE__H__

#define DEFCODE(__name__,__expression__,codes...) if(0){__name__:if(__expression__){codes}};
#define CONTROL(codes...) __CONTROL__: codes;

#define function(func_name,end_goto,__expression__,codes...) if(0){func_name:if(__expression__){codes}goto end_goto;};
#define used(func_name,end_goto) goto func_name;end_goto:

#endif  //!__COROUTINE__H__