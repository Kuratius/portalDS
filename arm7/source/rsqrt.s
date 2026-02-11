.syntax unified
.arm
.balign 4
.text
.global rsqrt_asm
.type rsqrt_asm, %function
rsqrt_asm: 
    //calculates rsqrt of 9.23 fixed point number in the range [1,4)
    mov    r1, #1073741824
    .irp x,1,2,3,4,5,6,7,8,9,10,11,12,13 //seems to be enough
    adds   r2,r0,r0,lsr #\x  
    addscc r2,r2,r2,lsr #\x
    movcc  r0,r2
    addcc  r1, r1,r1,lsr #\x
    .endr 
    umull  r2,r0, r1, r0
    adds   r1, r1,r1, lsr #1
    subs   r0, r1, r0, lsr #1
    bx lr
