# CS4280_Compilers
Compilers Projects

-The Language-

Identifiers: begin with letter, is alpha numeric
Integers: any sequence of decimal digits

Keywords:

	Begin End Start Stop Iff Looop
	Void Var Int Call Return Scan
	Print Program

Operators:

	== : + - * / & %

Relational Ops:

	= < > =!= >=> <=<

Delimiters:

	. ( ) , { } ; [ ]

Comments: starts with @ ends with a WS

-The BNF-

< program > -> < vars > < block >

< block > -> Begin < vars > < stats > End

< vars > -> Var Identifier < mvars > | E 

< mvars > -> ::Identifier < mvars > | E 

< expr > -> < M > < rpxe >

< rpxe > -> + < expr > | E

< M > -> < T > < N >

< N > -> - < M > | E

< T > -> < F > < X >

< X > -> * < T > | / < T > | E

< F > -> - < F > | < R >

< R > -> [ < expr > ] | Identifier | Number

< stats > -> < stat > < mStat >

< mStat > -> < stat > < mStat > | E

< stat > -> < in > | < out > | < block > | < if > | < loop > | < assign >

< in > -> Scan : Identifier .

< out > -> Print [ < expr > ] .

< if > -> [ < expr > < RO > < expr > ] Iff < block >

< loop > -> Loop [ < expr > < RO > < expr > ] < block >

< assign > -> Identifer = = < expr > .

< RO > -> >=> | <=< | = | > | < | =!=












