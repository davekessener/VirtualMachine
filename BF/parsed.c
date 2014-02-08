#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK 0x4000

void openFiles(int, char **, FILE **, FILE **);

int main(int argc, char *argv[])
{
	FILE *in, *out;
	unsigned char *ptr;

	openFiles(argc, argv, &in, &out);

	ptr = malloc(CHUNK);

	memset(ptr, 0, CHUNK);

	*ptr += 3;
	while(*ptr)
	{
		++ptr;
		*ptr += 5;
		--ptr;
		--*ptr;
	}
	ptr += 2;
	++*ptr;
	--ptr;
	while(*ptr)
	{
		ptr += 2;
		*ptr += 4;
		++ptr;
		*ptr += 2;
		++ptr;
		*ptr += 5;
		++ptr;
		*ptr += 5;
		++ptr;
		++*ptr;
		ptr += 2;
		++*ptr;
		--ptr;
		*ptr += 2;
		while(*ptr)
		{
			*ptr += 2;
			--ptr;
		}
		++ptr;
		*ptr -= 3;
	}
	++ptr;
	*ptr += 4;
	putc(*ptr, out);
	ptr += 3;
	putc(*ptr, out);
	*ptr += 5;
	putc(*ptr, out);
	++ptr;
	*ptr -= 6;
	putc(*ptr, out);
	--ptr;
	*ptr -= 2;
	putc(*ptr, out);
	*ptr += 9;
	putc(*ptr, out);
	++ptr;
	++*ptr;
	putc(*ptr, out);
	++*ptr;
	putc(*ptr, out);
	ptr -= 4;
	*ptr -= 3;
	putc(*ptr, out);
	while(*ptr)
	{
		++ptr;
	}
	ptr -= 2;
	putc(*ptr, out);
	ptr -= 3;
	putc(*ptr, out);
	*ptr -= 7;
	putc(*ptr, out);
	++ptr;
	*ptr += 4;
	putc(*ptr, out);
	--ptr;
	*ptr += 5;
	putc(*ptr, out);
	++*ptr;
	putc(*ptr, out);
	++ptr;
	*ptr -= 5;
	putc(*ptr, out);
	++ptr;
	++*ptr;
	putc(*ptr, out);
	--ptr;
	*ptr += 4;
	putc(*ptr, out);
	ptr += 2;
	*ptr += 2;
	putc(*ptr, out);
	++ptr;
	*ptr -= 5;
	putc(*ptr, out);
	ptr -= 3;
	*ptr -= 5;
	putc(*ptr, out);
	*ptr += 5;
	putc(*ptr, out);
	*ptr -= 7;
	putc(*ptr, out);
	--ptr;
	*ptr -= 2;
	putc(*ptr, out);
	ptr -= 3;
	putc(*ptr, out);
	ptr += 3;
	putc(*ptr, out);
	ptr -= 2;
	++*ptr;
	putc(*ptr, out);
	++ptr;
	*ptr -= 6;
	putc(*ptr, out);
	--*ptr;
	putc(*ptr, out);
	putc(*ptr, out);
	*ptr -= 2;
	putc(*ptr, out);
	*ptr += 3;
	putc(*ptr, out);
	*ptr -= 5;
	--ptr;
	*ptr += 2;
	putc(*ptr, out);
	--ptr;
	*ptr -= 2;
	while(*ptr)
	{
		++ptr;
		++*ptr;
		--ptr;
		--*ptr;
	}
	ptr += 5;
	*ptr -= 2;
	putc(*ptr, out);
	*ptr -= 2;
	putc(*ptr, out);
	--ptr;
	*ptr += 4;
	putc(*ptr, out);
	ptr += 2;
	--*ptr;
	putc(*ptr, out);
	ptr -= 3;
	putc(*ptr, out);
	ptr += 3;
	*ptr -= 2;
	putc(*ptr, out);
	++ptr;
	putc(*ptr, out);
	ptr -= 4;
	*ptr -= 5;
	putc(*ptr, out);
	++ptr;
	*ptr -= 4;
	putc(*ptr, out);
	*ptr += 8;
	putc(*ptr, out);
	*ptr -= 4;
	--ptr;
	++*ptr;
	putc(*ptr, out);
	*ptr += 9;
	ptr += 2;
	*ptr -= 2;
	putc(*ptr, out);
	++*ptr;
	putc(*ptr, out);
	*ptr += 2;
	ptr -= 4;
	putc(*ptr, out);
	while(*ptr)
	{
		++ptr;
	}
	--ptr;
	putc(*ptr, out);
	ptr += 2;
	*ptr = getc(in);
	while(*ptr)
	{
		ptr += 2;
		*ptr += 3;
		while(*ptr)
		{
			--ptr;
			*ptr += 7;
			++ptr;
			--*ptr;
		}
		--ptr;
		while(*ptr)
		{
			--ptr;
			while(*ptr)
			{
				--*ptr;
				while(*ptr)
				{
					--*ptr;
					--ptr;
				}
			}
			ptr += 2;
			while(*ptr)
			{
				++ptr;
			}
			--ptr;
			--*ptr;
		}
		--ptr;
		while(*ptr)
		{
			--ptr;
			*ptr += 5;
			++ptr;
			--*ptr;
			while(*ptr)
			{
				--ptr;
				*ptr += 3;
				++ptr;
				--*ptr;
				while(*ptr)
				{
					--ptr;
					*ptr -= 2;
					++ptr;
					--*ptr;
					while(*ptr)
					{
						--ptr;
						*ptr += 3;
						++ptr;
						--*ptr;
						while(*ptr)
						{
							--ptr;
							*ptr += 4;
							while(*ptr)
							{
								++ptr;
								while(*ptr)
								{
									--*ptr;
									ptr += 2;
								}
								--ptr;
								while(*ptr)
								{
									ptr += 2;
								}
								ptr -= 2;
								--*ptr;
							}
							++ptr;
							while(*ptr)
							{
								--ptr;
								*ptr += 3;
								++ptr;
								--*ptr;
								while(*ptr)
								{
									--ptr;
									*ptr -= 3;
									++ptr;
									--*ptr;
									while(*ptr)
									{
										--ptr;
										*ptr += 4;
										++ptr;
										--*ptr;
										while(*ptr)
										{
											--ptr;
											*ptr += 3;
											while(*ptr)
											{
												++ptr;
												while(*ptr)
												{
													--*ptr;
													while(*ptr)
													{
														--*ptr;
														while(*ptr)
														{
															--*ptr;
															while(*ptr)
															{
																--*ptr;
																ptr += 2;
															}
														}
													}
												}
												--ptr;
												while(*ptr)
												{
													ptr += 2;
												}
												ptr -= 2;
												--*ptr;
											}
											++ptr;
											while(*ptr)
											{
												--ptr;
												++*ptr;
												++ptr;
												--*ptr;
												while(*ptr)
												{
													--ptr;
													--*ptr;
													++ptr;
													--*ptr;
													while(*ptr)
													{
														--ptr;
														*ptr += 2;
														++ptr;
														--*ptr;
														while(*ptr)
														{
															--ptr;
															while(*ptr)
															{
																--*ptr;
															}
															++ptr;
															--*ptr;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		--ptr;
		while(*ptr)
		{
			--*ptr;
			while(*ptr)
			{
				--*ptr;
				while(*ptr)
				{
					++ptr;
					++*ptr;
					--ptr;
					--*ptr;
				}
				++ptr;
			}
			--ptr;
			while(*ptr)
			{
				ptr -= 4;
				putc(*ptr, out);
				++ptr;
				*ptr += 3;
				putc(*ptr, out);
				++*ptr;
				putc(*ptr, out);
				*ptr += 3;
				putc(*ptr, out);
				*ptr -= 7;
				putc(*ptr, out);
				++ptr;
				*ptr -= 3;
				putc(*ptr, out);
				*ptr += 2;
				putc(*ptr, out);
				--ptr;
				putc(*ptr, out);
				++ptr;
				--*ptr;
				putc(*ptr, out);
				*ptr += 2;
				ptr -= 4;
				putc(*ptr, out);
				while(*ptr)
				{
					++ptr;
				}
				ptr += 9;
			}
			--ptr;
			while(*ptr)
			{
				while(*ptr)
				{
					--ptr;
				}
				++ptr;
				*ptr += 2;
				putc(*ptr, out);
				*ptr -= 2;
				while(*ptr)
				{
					++ptr;
				}
				ptr += 8;
			}
			--ptr;
			while(*ptr)
			{
				ptr -= 2;
				*ptr += 2;
				putc(*ptr, out);
				putc(*ptr, out);
				*ptr -= 2;
				ptr += 6;
			}
			--ptr;
			while(*ptr)
			{
				ptr -= 2;
				putc(*ptr, out);
				putc(*ptr, out);
				ptr += 5;
			}
			--ptr;
			while(*ptr)
			{
				ptr -= 2;
				putc(*ptr, out);
				putc(*ptr, out);
				--*ptr;
				putc(*ptr, out);
				++*ptr;
				ptr += 4;
			}
			--ptr;
			while(*ptr)
			{
				ptr -= 2;
				*ptr += 2;
				putc(*ptr, out);
				putc(*ptr, out);
				*ptr -= 3;
				putc(*ptr, out);
				++*ptr;
				ptr += 3;
			}
			--ptr;
			while(*ptr)
			{
				ptr -= 3;
				putc(*ptr, out);
				ptr += 2;
				putc(*ptr, out);
				ptr += 5;
			}
			--ptr;
			while(*ptr)
			{
				ptr -= 4;
				*ptr -= 5;
				putc(*ptr, out);
				*ptr += 5;
				++ptr;
				putc(*ptr, out);
				*ptr -= 4;
				putc(*ptr, out);
				*ptr += 3;
				putc(*ptr, out);
				++*ptr;
				++ptr;
				*ptr -= 3;
				putc(*ptr, out);
				ptr -= 3;
				--*ptr;
				putc(*ptr, out);
				while(*ptr)
				{
					++ptr;
				}
				++ptr;
			}
			--ptr;
			while(*ptr)
			{
				ptr -= 4;
				putc(*ptr, out);
				*ptr -= 5;
				putc(*ptr, out);
				++ptr;
				*ptr += 4;
				putc(*ptr, out);
				--ptr;
				*ptr += 2;
				putc(*ptr, out);
				*ptr += 3;
				++ptr;
				*ptr -= 4;
				putc(*ptr, out);
				++ptr;
				*ptr -= 3;
				putc(*ptr, out);
				ptr -= 3;
				putc(*ptr, out);
				--*ptr;
				while(*ptr)
				{
					++ptr;
				}
			}
			--ptr;
			while(*ptr)
			{
				ptr -= 5;
				*ptr -= 4;
				putc(*ptr, out);
				ptr += 2;
				putc(*ptr, out);
				ptr -= 2;
				putc(*ptr, out);
				*ptr += 5;
				putc(*ptr, out);
				ptr += 3;
				++*ptr;
				putc(*ptr, out);
				*ptr += 2;
				++ptr;
				putc(*ptr, out);
				ptr += 2;
			}
			--ptr;
			putc(*ptr, out);
			++ptr;
		}
		++ptr;
		*ptr = getc(in);
	}
	ptr -= 5;
	putc(*ptr, out);
	--ptr;
	++*ptr;
	putc(*ptr, out);
	++ptr;
	*ptr += 4;
	putc(*ptr, out);
	--ptr;
	*ptr -= 4;
	putc(*ptr, out);
	ptr += 2;
	*ptr -= 3;
	putc(*ptr, out);
	ptr -= 3;
	--*ptr;
	putc(*ptr, out);
	ptr += 3;
	++*ptr;
	putc(*ptr, out);
	++ptr;
	putc(*ptr, out);
	++ptr;
	putc(*ptr, out);
	while(*ptr)
	{
		--ptr;
	}
	++ptr;
	*ptr += 2;
	putc(*ptr, out);
	while(*ptr)
	{
		++ptr;
	}
	--ptr;
	putc(*ptr, out);
	++ptr;
	while(*ptr)
	{
		putc(*ptr, out);
		*ptr -= 4;
		++ptr;
		putc(*ptr, out);
		*ptr -= 3;
		*ptr = getc(in);
		putc(*ptr, out);
		putc(*ptr, out);
		putc(*ptr, out);
		putc(*ptr, out);
	}

	return 0;
}

void openFiles(int argc, char *argv[], FILE **in, FILE **out)
{
	*in = stdin;
	*out = stdout;
}
