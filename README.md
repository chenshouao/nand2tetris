# nand2tetris

This repository is for recording the learning of nand2tetris, which is for increasing my core literacy.



## project-00

熟悉作业提交形式，交文件要交压缩包。

## project-01

主要是基于与非门搭建15个常用门电路，前几个常见的不难，跟解调/译码相关的不是那么好想。



##### 单路Multiplexor

```vhdl
out = a if sel == 0
	  b otherwise
```

```c
out  = (a&(!sel))|(b&sel)
```

```vhdl
CHIP Mux {
    IN a, b, sel;
    OUT out;
    PARTS:
    // Put your code here:
	Not(in = sel, out = Nsel);
	And(a = a, b = Nsel, out = aMask);
	And(a = b, b = sel, out = bMask);
	Or(a = aMask, b = bMask, out = out);
}
```

##### 16位单路Multiplexor

```vhdl
CHIP Mux16 {
    IN a[16], b[16], sel;
    OUT out[16];

    PARTS:
    // Put your code here:
	Mux(a = a[0], b = b[0], sel = sel,  out = out[0]);
	Mux(a = a[1], b = b[1], sel = sel,  out = out[1]);
	Mux(a = a[2], b = b[2], sel = sel,  out = out[2]);
	Mux(a = a[3], b = b[3], sel = sel,  out = out[3]);
	Mux(a = a[4], b = b[4], sel = sel,  out = out[4]);
	Mux(a = a[5], b = b[5], sel = sel,  out = out[5]);
	Mux(a = a[6], b = b[6], sel = sel,  out = out[6]);
	Mux(a = a[7], b = b[7], sel = sel,  out = out[7]);
	Mux(a = a[8], b = b[8], sel = sel,  out = out[8]);
	Mux(a = a[9], b = b[9], sel = sel,  out = out[9]);
	Mux(a = a[10], b = b[10], sel = sel,  out = out[10]);
	Mux(a = a[11], b = b[11], sel = sel,  out = out[11]);
	Mux(a = a[12], b = b[12], sel = sel,  out = out[12]);
	Mux(a = a[13], b = b[13], sel = sel,  out = out[13]);
	Mux(a = a[14], b = b[14], sel = sel,  out = out[14]);
	Mux(a = a[15], b = b[15], sel = sel,  out = out[15]);
}
```

##### 4路16位Multiplexor

```vhdl
CHIP Mux4Way16 {
    IN a[16], b[16], c[16], d[16], sel[2];
    OUT out[16];

    PARTS:
    // Put your code here:
	Mux16(a = a, b = b, sel = sel[0], out = ab);
	Mux16(a = c, b = d, sel = sel[0], out = cd);
	Mux16(a = ab, b = cd, sel = sel[1], out = out);
}
```

##### 8路16位Multiplexor

```vhdl
CHIP Mux8Way16 {
    IN a[16], b[16], c[16], d[16],
       e[16], f[16], g[16], h[16],
       sel[3];
    OUT out[16];

    PARTS:
    // Put your code here:
	Mux4Way16(a = a, b = b, c = c, d = d, sel = sel[0..1], out = abcd);
	Mux4Way16(a = e, b = f, c = g, d = h, sel = sel[0..1], out = efgh);
	Mux16(a = abcd, b = efgh, sel = sel[2], out = out);
}
```

##### 单路DeMultiplexor

```vhdl
{a, b} = {in, 0} if sel == 0
		 {0, in} if sel == 1
```

```c
a = in&(!sel)
b = in&sel
```

```vhdl
CHIP DMux {
    IN in, sel;
    OUT a, b;

    PARTS:
    // Put your code here:
	Not(in = sel, out = Nsel);
	And(a = in, b = Nsel, out = a);
	And(a = in, b = sel, out = b);
}
```

##### 4路DeMultiplexor

```vhdl
CHIP DMux4Way {
    IN in, sel[2];
    OUT a, b, c, d;

    PARTS:
    Not(in = sel[0], out = Nsel0);
	Not(in = sel[1], out = Nsel1);
	DMux(in = in, sel = sel[0], a = ac, b = bd);
	DMux(in = ac, sel = sel[1], a = a, b = c);
	DMux(in = bd, sel = sel[1], a = b, b = d);
}
```

##### 8路DeMultiplexor

```vhdl
CHIP DMux8Way {
    IN in, sel[3];
    OUT a, b, c, d, e, f, g, h;

    PARTS:
    // Put your code here:
	DMux4Way(in = in, sel = sel[0..1], a = ae, b = bf, c = cg, d = dh);
	DMux(in = ae, sel = sel[2], a = a, b = e);
	DMux(in = bf, sel = sel[2], a = b, b = f);
	DMux(in = cg, sel = sel[2], a = c, b = g);
	DMux(in = dh, sel = sel[2], a = d, b = h);
}
```



本节project充分体现了计算机科学中的一个重要思维即层层抽象，调用低一层的接口实现高层，同时也把减而治之和分而治之的思想体现的淋漓尽致。



## project02

##### 半加器

a xor b 的结果是不进位的加法，进位用 a and b 计算。

```vhdl
CHIP HalfAdder {
    IN a, b;    // 1-bit inputs
    OUT sum,    // Right bit of a + b 
        carry;  // Left bit of a + b

    PARTS:
    // Put you code here:
	Xor(a = a, b = b, out = sum);
	And(a = a, b = b, out = carry);
}
```



##### 全加器

用两个半加器实现，注意进位要用两个进位或。(3个bit相加不可能进位两次)

```vhdl
CHIP FullAdder {
    IN a, b, c;  // 1-bit inputs
    OUT sum,     // Right bit of a + b + c
        carry;   // Left bit of a + b + c
    PARTS:
    // Put you code here:
	HalfAdder(a = a, b = b, sum = aAddb, carry = carry1);
	HalfAdder(a = aAddb, b = c, sum = sum, carry = carry2);
	Or(a = carry1, b = carry2, out = carry);
}
```



##### 加法器

串行进位加法器，并没有实现超前进位的，所以效率会比较低。

```vhdl
CHIP Add16 {
    IN a[16], b[16];
    OUT out[16];

    PARTS:
   // Put you code here:
	HalfAdder(a = a[0], b = b[0], sum = out[0], carry = carry0);
	FullAdder(a = a[1], b = b[1], c = carry0, sum = out[1], carry = carry1);
	FullAdder(a = a[2], b = b[2], c = carry1, sum = out[2], carry = carry2);
	FullAdder(a = a[3], b = b[3], c = carry2, sum = out[3], carry = carry3);
	FullAdder(a = a[4], b = b[4], c = carry3, sum = out[4], carry = carry4);
	FullAdder(a = a[5], b = b[5], c = carry4, sum = out[5], carry = carry5);
	FullAdder(a = a[6], b = b[6], c = carry5, sum = out[6], carry = carry6);
	FullAdder(a = a[7], b = b[7], c = carry6, sum = out[7], carry = carry7);
	FullAdder(a = a[8], b = b[8], c = carry7, sum = out[8], carry = carry8);
	FullAdder(a = a[9], b = b[9], c = carry8, sum = out[9], carry = carry9);
	FullAdder(a = a[10], b = b[10], c = carry9, sum = out[10], carry = carry10);
	FullAdder(a = a[11], b = b[11], c = carry10, sum = out[11], carry = carry11);
	FullAdder(a = a[12], b = b[12], c = carry11, sum = out[12], carry = carry12);
	FullAdder(a = a[13], b = b[13], c = carry12, sum = out[13], carry = carry13);
	FullAdder(a = a[14], b = b[14], c = carry13, sum = out[14], carry = carry14);
	FullAdder(a = a[15], b = b[15], c = carry14, sum = out[15], carry = carry15);
}
```

##### 增1器

用16位加法器的思路实现。

```vhdl
CHIP Inc16 {
    IN in[16];
    OUT out[16];

    PARTS:
   // Put you code here:
	HalfAdder(a = in[0], b = true, sum = out[0], carry = carry0);
	FullAdder(a = in[1], b = false, c = carry0, sum = out[1], carry = carry1);
	FullAdder(a = in[2], b = false, c = carry1, sum = out[2], carry = carry2);
	FullAdder(a = in[3], b = false, c = carry2, sum = out[3], carry = carry3);
	FullAdder(a = in[4], b = false, c = carry3, sum = out[4], carry = carry4);
	FullAdder(a = in[5], b = false, c = carry4, sum = out[5], carry = carry5);
	FullAdder(a = in[6], b = false, c = carry5, sum = out[6], carry = carry6);
	FullAdder(a = in[7], b = false, c = carry6, sum = out[7], carry = carry7);
	FullAdder(a = in[8], b = false, c = carry7, sum = out[8], carry = carry8);
	FullAdder(a = in[9], b = false, c = carry8, sum = out[9], carry = carry9);
	FullAdder(a = in[10], b = false, c = carry9, sum = out[10], carry = carry10);
	FullAdder(a = in[11], b = false, c = carry10, sum = out[11], carry = carry11);
	FullAdder(a = in[12], b = false, c = carry11, sum = out[12], carry = carry12);
	FullAdder(a = in[13], b = false, c = carry12, sum = out[13], carry = carry13);
	FullAdder(a = in[14], b = false, c = carry13, sum = out[14], carry = carry14);
	FullAdder(a = in[15], b = false, c = carry14, sum = out[15], carry = carry15);
}
```

##### ALU

比较重量级，我把16位分开写的，写了200行左右，不过我是先写一行，再用C++生成16行不同参数的。

其实要实现的核心内容也不多：

1.取零:与0与

2.取反:与1异或

3.是否为负数？符号位

4.一个重要的等式：X + Y = X xor Y + 2 * X and Y，于是初始结果为 X and Y, 再加上 X and Y + X xor Y的每一位与f与运算后的结果。

(本质是因为异或是不带进位的加法，而与是进位的位) 

```vhdl
CHIP ALU {
    IN  
        x[16], y[16],  // 16-bit inputs        
        zx, // zero the x input?
        nx, // negate the x input?
        zy, // zero the y input?
        ny, // negate the y input?
        f,  // compute out = x + y (if 1) or x & y (if 0)
        no; // negate the out output?

    OUT 
        out[16], // 16-bit output
        zr, // 1 if (out == 0), 0 otherwise
        ng; // 1 if (out < 0),  0 otherwise

    PARTS:
   // Put you code here:
    Not(in = zx, out = Notzx);
	And(a = x[0], b = Notzx, out = newx0);
	And(a = x[1], b = Notzx, out = newx1);
	And(a = x[2], b = Notzx, out = newx2);
	And(a = x[3], b = Notzx, out = newx3);
	And(a = x[4], b = Notzx, out = newx4);
	And(a = x[5], b = Notzx, out = newx5);
	And(a = x[6], b = Notzx, out = newx6);
	And(a = x[7], b = Notzx, out = newx7);
	And(a = x[8], b = Notzx, out = newx8);
	And(a = x[9], b = Notzx, out = newx9);
	And(a = x[10], b = Notzx, out = newx10);
	And(a = x[11], b = Notzx, out = newx11);
	And(a = x[12], b = Notzx, out = newx12);
	And(a = x[13], b = Notzx, out = newx13);
	And(a = x[14], b = Notzx, out = newx14);
	And(a = x[15], b = Notzx, out = newx15);
	
	Xor(a = newx0, b = nx, out = finalx0);
	Xor(a = newx1, b = nx, out = finalx1);
	Xor(a = newx2, b = nx, out = finalx2);
	Xor(a = newx3, b = nx, out = finalx3);
	Xor(a = newx4, b = nx, out = finalx4);
	Xor(a = newx5, b = nx, out = finalx5);
	Xor(a = newx6, b = nx, out = finalx6);
	Xor(a = newx7, b = nx, out = finalx7);
	Xor(a = newx8, b = nx, out = finalx8);
	Xor(a = newx9, b = nx, out = finalx9);
	Xor(a = newx10, b = nx, out = finalx10);
	Xor(a = newx11, b = nx, out = finalx11);
	Xor(a = newx12, b = nx, out = finalx12);
	Xor(a = newx13, b = nx, out = finalx13);
	Xor(a = newx14, b = nx, out = finalx14);
	Xor(a = newx15, b = nx, out = finalx15);
	//
	Not(in = zy, out = Notzy);
	And(a = y[0], b = Notzy, out = newy0);
	And(a = y[1], b = Notzy, out = newy1);
	And(a = y[2], b = Notzy, out = newy2);
	And(a = y[3], b = Notzy, out = newy3);
	And(a = y[4], b = Notzy, out = newy4);
	And(a = y[5], b = Notzy, out = newy5);
	And(a = y[6], b = Notzy, out = newy6);
	And(a = y[7], b = Notzy, out = newy7);
	And(a = y[8], b = Notzy, out = newy8);
	And(a = y[9], b = Notzy, out = newy9);
	And(a = y[10], b = Notzy, out = newy10);
	And(a = y[11], b = Notzy, out = newy11);
	And(a = y[12], b = Notzy, out = newy12);
	And(a = y[13], b = Notzy, out = newy13);
	And(a = y[14], b = Notzy, out = newy14);
	And(a = y[15], b = Notzy, out = newy15);
	
	Xor(a = newy0, b = ny, out = finaly0);
	Xor(a = newy1, b = ny, out = finaly1);
	Xor(a = newy2, b = ny, out = finaly2);
	Xor(a = newy3, b = ny, out = finaly3);
	Xor(a = newy4, b = ny, out = finaly4);
	Xor(a = newy5, b = ny, out = finaly5);
	Xor(a = newy6, b = ny, out = finaly6);
	Xor(a = newy7, b = ny, out = finaly7);
	Xor(a = newy8, b = ny, out = finaly8);
	Xor(a = newy9, b = ny, out = finaly9);
	Xor(a = newy10, b = ny, out = finaly10);
	Xor(a = newy11, b = ny, out = finaly11);
	Xor(a = newy12, b = ny, out = finaly12);
	Xor(a = newy13, b = ny, out = finaly13);
	Xor(a = newy14, b = ny, out = finaly14);
	Xor(a = newy15, b = ny, out = finaly15);
	
	Xor(a = finalx0, b = finaly0, out = xorSum0);
	Xor(a = finalx1, b = finaly1, out = xorSum1);
	Xor(a = finalx2, b = finaly2, out = xorSum2);
	Xor(a = finalx3, b = finaly3, out = xorSum3);
	Xor(a = finalx4, b = finaly4, out = xorSum4);
	Xor(a = finalx5, b = finaly5, out = xorSum5);
	Xor(a = finalx6, b = finaly6, out = xorSum6);
	Xor(a = finalx7, b = finaly7, out = xorSum7);
	Xor(a = finalx8, b = finaly8, out = xorSum8);
	Xor(a = finalx9, b = finaly9, out = xorSum9);
	Xor(a = finalx10, b = finaly10, out = xorSum10);
	Xor(a = finalx11, b = finaly11, out = xorSum11);
	Xor(a = finalx12, b = finaly12, out = xorSum12);
	Xor(a = finalx13, b = finaly13, out = xorSum13);
	Xor(a = finalx14, b = finaly14, out = xorSum14);
	Xor(a = finalx15, b = finaly15, out = xorSum15);
	
	And(a = finalx0, b = finaly0, out = sum0);
	And(a = finalx1, b = finaly1, out = sum1);
	And(a = finalx2, b = finaly2, out = sum2);
	And(a = finalx3, b = finaly3, out = sum3);
	And(a = finalx4, b = finaly4, out = sum4);
	And(a = finalx5, b = finaly5, out = sum5);
	And(a = finalx6, b = finaly6, out = sum6);
	And(a = finalx7, b = finaly7, out = sum7);
	And(a = finalx8, b = finaly8, out = sum8);
	And(a = finalx9, b = finaly9, out = sum9);
	And(a = finalx10, b = finaly10, out = sum10);
	And(a = finalx11, b = finaly11, out = sum11);
	And(a = finalx12, b = finaly12, out = sum12);
	And(a = finalx13, b = finaly13, out = sum13);
	And(a = finalx14, b = finaly14, out = sum14);
	And(a = finalx15, b = finaly15, out = sum15);
	
	Add16(a[0] = sum0,a[1] = sum1,a[2] = sum2,a[3] = sum3,a[4] = sum4,a[5] = sum5,a[6] = sum6,a[7] = sum7,a[8] = sum8,a[9] = sum9,a[10] = sum10,a[11] = sum11,a[12] = sum12,a[13] = sum13,a[14] = sum14,a[15] = sum15,b[0] = xorSum0,b[1] = xorSum1,b[2] = xorSum2,b[3] = xorSum3,b[4] = xorSum4,b[5] = xorSum5,b[6] = xorSum6,b[7] = xorSum7,b[8] = xorSum8,b[9] = xorSum9,b[10] = xorSum10,b[11] = xorSum11,b[12] = xorSum12,b[13] = xorSum13,b[14] = xorSum14,b[15] = xorSum15,out[0] = tempSum0,out[1] = tempSum1,out[2] = tempSum2,out[3] = tempSum3,out[4] = tempSum4,out[5] = tempSum5,out[6] = tempSum6,out[7] = tempSum7,out[8] = tempSum8,out[9] = tempSum9,out[10] = tempSum10,out[11] = tempSum11,out[12] = tempSum12,out[13] = tempSum13,out[14] = tempSum14,out[15] = tempSum15);
	
	And(a = tempSum0, b = f, out = carry0);
	And(a = tempSum1, b = f, out = carry1);
	And(a = tempSum2, b = f, out = carry2);
	And(a = tempSum3, b = f, out = carry3);
	And(a = tempSum4, b = f, out = carry4);
	And(a = tempSum5, b = f, out = carry5);
	And(a = tempSum6, b = f, out = carry6);
	And(a = tempSum7, b = f, out = carry7);
	And(a = tempSum8, b = f, out = carry8);
	And(a = tempSum9, b = f, out = carry9);
	And(a = tempSum10, b = f, out = carry10);
	And(a = tempSum11, b = f, out = carry11);
	And(a = tempSum12, b = f, out = carry12);
	And(a = tempSum13, b = f, out = carry13);
	And(a = tempSum14, b = f, out = carry14);
	And(a = tempSum15, b = f, out = carry15);
	
	Add16(a[0] = sum0,a[1] = sum1,a[2] = sum2,a[3] = sum3,a[4] = sum4,a[5] = sum5,a[6] = sum6,a[7] = sum7,a[8] = sum8,a[9] = sum9,a[10] = sum10,a[11] = sum11,a[12] = sum12,a[13] = sum13,a[14] = sum14,a[15] = sum15,b[0] = carry0,b[1] = carry1,b[2] = carry2,b[3] = carry3,b[4] = carry4,b[5] = carry5,b[6] = carry6,b[7] = carry7,b[8] = carry8,b[9] = carry9,b[10] = carry10,b[11] = carry11,b[12] = carry12,b[13] = carry13,b[14] = carry14,b[15] = carry15,out[0] = tmpOut0,out[1] = tmpOut1,out[2] = tmpOut2,out[3] = tmpOut3,out[4] = tmpOut4,out[5] = tmpOut5,out[6] = tmpOut6,out[7] = tmpOut7,out[8] = tmpOut8,out[9] = tmpOut9,out[10] = tmpOut10,out[11] = tmpOut11,out[12] = tmpOut12,out[13] = tmpOut13,out[14] = tmpOut14,out[15] = tmpOut15);
	
	Xor(a = tmpOut0, b = no, out = out0);
	Xor(a = tmpOut1, b = no, out = out1);
	Xor(a = tmpOut2, b = no, out = out2);
	Xor(a = tmpOut3, b = no, out = out3);
	Xor(a = tmpOut4, b = no, out = out4);
	Xor(a = tmpOut5, b = no, out = out5);
	Xor(a = tmpOut6, b = no, out = out6);
	Xor(a = tmpOut7, b = no, out = out7);
	Xor(a = tmpOut8, b = no, out = out8);
	Xor(a = tmpOut9, b = no, out = out9);
	Xor(a = tmpOut10, b = no, out = out10);
	Xor(a = tmpOut11, b = no, out = out11);
	Xor(a = tmpOut12, b = no, out = out12);
	Xor(a = tmpOut13, b = no, out = out13);
	Xor(a = tmpOut14, b = no, out = out14);
	Xor(a = tmpOut15, b = no, out = out15);
	
	Or8Way(in[0] = out0,in[1] = out1,in[2] = out2,in[3] = out3,in[4] = out4,in[5] = out5,in[6] = out6,in[7] = out7, out = orOut1);
	Or8Way(in[0] = out8,in[1] = out9,in[2] = out10,in[3] = out11,in[4] = out12,in[5] = out13,in[6] = out14,in[7] = out15, out = orOut2);
	Or(a = orOut1, b = orOut2, out = orOut3);
	Not(in = orOut3, out = zr);
	Or(a = false, b = out15, out = ng);
	
	Or(a = false, b = out0, out = out[0]);
	Or(a = false, b = out1, out = out[1]);
	Or(a = false, b = out2, out = out[2]);
	Or(a = false, b = out3, out = out[3]);
	Or(a = false, b = out4, out = out[4]);
	Or(a = false, b = out5, out = out[5]);
	Or(a = false, b = out6, out = out[6]);
	Or(a = false, b = out7, out = out[7]);
	Or(a = false, b = out8, out = out[8]);
	Or(a = false, b = out9, out = out[9]);
	Or(a = false, b = out10, out = out[10]);
	Or(a = false, b = out11, out = out[11]);
	Or(a = false, b = out12, out = out[12]);
	Or(a = false, b = out13, out = out[13]);
	Or(a = false, b = out14, out = out[14]);
	Or(a = false, b = out15, out = out[15]);
}
```

## project03

##### 位存储单元Bit

利用D触发器实现时序逻辑(将D触发器作为基本元件使用)，当load位为1时，输出为in，否则为上一个时钟的out。可以作为基本的存储位使用。

```vhdl
CHIP Bit {
    IN in, load;
    OUT out;

    PARTS:
    // Put your code here:
	Mux(a = dffout, b = in, sel = load, out = muxOut);
	DFF(in = muxOut, out = dffout, out = out);
}
```

##### 寄存器Register

```vhdl
/**
 * 16-bit register:
 * If load[t] == 1 then out[t+1] = in[t]
 * else out does not change
 */

CHIP Register {
    IN in[16], load;
    OUT out[16];

    PARTS:
    // Put your code here:
	Bit(in = in[0], load = load, out = out[0]);
	Bit(in = in[1], load = load, out = out[1]);
	Bit(in = in[2], load = load, out = out[2]);
	Bit(in = in[3], load = load, out = out[3]);
	Bit(in = in[4], load = load, out = out[4]);
	Bit(in = in[5], load = load, out = out[5]);
	Bit(in = in[6], load = load, out = out[6]);
	Bit(in = in[7], load = load, out = out[7]);
	Bit(in = in[8], load = load, out = out[8]);
	Bit(in = in[9], load = load, out = out[9]);
	Bit(in = in[10], load = load, out = out[10]);
	Bit(in = in[11], load = load, out = out[11]);
	Bit(in = in[12], load = load, out = out[12]);
	Bit(in = in[13], load = load, out = out[13]);
	Bit(in = in[14], load = load, out = out[14]);
	Bit(in = in[15], load = load, out = out[15]);
}
```

##### 程序计数器PC

根据状态字选择最后的值，最后load到寄存器中实现时序逻辑。

```vhdl
/**
 * A 16-bit counter with load and reset control bits.
 * if      (reset[t] == 1) out[t+1] = 0
 * else if (load[t] == 1)  out[t+1] = in[t]
 * else if (inc[t] == 1)   out[t+1] = out[t] + 1  (integer addition)
 * else                    out[t+1] = out[t]
 */

CHIP PC {
    IN in[16],load,inc,reset;
    OUT out[16];

    PARTS:
    // Put your code here:
	Inc16(in = finalOut, out = incOut);
	Mux16(a = finalOut, b = incOut, sel = inc, out = firstOut);
	Mux16(a = firstOut, b = in, sel = load, out = secondOut);
	Mux16(a = secondOut, b[0] = false,b[1] = false,b[2] = false,b[3] = false,b[4] = false,b[5] = false,b[6] = false,b[7] = false,
		b[8] = false,b[9] = false,b[10] = false,b[11] = false,b[12] = false,b[13] = false,b[14] = false,b[15] = false, sel = reset, out = thirdOut);
	Register(in = thirdOut, load = true, out = finalOut, out = out);
}
```

##### 存储芯片RAM

主要是译码器的使用，最后一层层用8-3/3-8译码器和4-2/2-4译码器嵌套到了16k的RAM。

```vhdl
/**
 * Memory of 8 registers, each 16 bit-wide. Out holds the value
 * stored at the memory location specified by address. If load==1, then 
 * the in value is loaded into the memory location specified by address 
 * (the loaded value will be emitted to out from the next time step onward).
 */

CHIP RAM8 {
    IN in[16], load, address[3];
    OUT out[16];

    PARTS:
    // Put your code here:
	DMux8Way(in = load, sel = address, a = load0,  b = load1,  c = load2,  d = load3,  e = load4,  f = load5,  g = load6, h = load7);
	Register(in = in, load = load0, out = out0);
	Register(in = in, load = load1, out = out1);
	Register(in = in, load = load2, out = out2);
	Register(in = in, load = load3, out = out3);
	Register(in = in, load = load4, out = out4);
	Register(in = in, load = load5, out = out5);
	Register(in = in, load = load6, out = out6);
	Register(in = in, load = load7, out = out7);
	Mux8Way16(a = out0,  b = out1,  c = out2,  d = out3,  e = out4,  f = out5,  g = out6, h = out7, sel = address, out = out);
}
```

```vhdl
CHIP RAM16K {
    IN in[16], load, address[14];
    OUT out[16];

    PARTS:
    // Put your code here:
	DMux4Way(in = load, sel = address[12..13], a = load0,  b = load1,  c = load2,  d = load3);
	RAM4K(in = in, load = load0, address = address[0..11], out = out0);
	RAM4K(in = in, load = load1, address = address[0..11], out = out1);
	RAM4K(in = in, load = load2, address = address[0..11], out = out2);
	RAM4K(in = in, load = load3, address = address[0..11], out = out3);
	Mux4Way16(a = out0,  b = out1,  c = out2,  d = out3, sel = address[12..13], out = out);
}
```