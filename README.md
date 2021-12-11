# nand2tetris

This repository is for recording the learning of nand2tetris, which is for increasing my core literacy.



## project-00

熟悉作业提交形式，交文件要交压缩包。

## project-01

主要是基于与非门搭建15个常用门电路，前几个常见的不难，跟解调/译码相关的不是那么好想。



单路Multiplexor

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

16位单路Multiplexor

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

4路16位Multiplexor

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

8路16位Multiplexor

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

单路DeMultiplexor

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

4路DeMultiplexor

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

8路DeMultiplexor

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