%%

a	{ double a = .0; printf("1"); REJECT; }
a	{ printf("2"); REJECT; }
a	{ printf("3"); REJECT; }

