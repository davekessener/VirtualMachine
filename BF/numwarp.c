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

	ptr += 4;
	++*ptr;
	++ptr;
	*ptr += 3;
	++ptr;
	*ptr += 3;
	ptr += 5;
	*ptr += 3;
	while(*ptr)
	{
		++ptr;
		*ptr = getc(in);
		++*ptr;
		++ptr;
		*ptr += 4;
		while(*ptr)
		{
			++ptr;
			*ptr += 4;
			--ptr;
			--*ptr;
		}
		++ptr;
		while(*ptr)
		{
			ptr -= 2;
			while(*ptr)
			{
				--*ptr;
				while(*ptr)
				{
					--*ptr;
					++ptr;
				}
			}
			++ptr;
			while(*ptr)
			{
				--ptr;
			}
			++ptr;
			--*ptr;
		}
		ptr -= 2;
		while(*ptr)
		{
			++ptr;
			++*ptr;
			++ptr;
			++*ptr;
			ptr += 2;
			++*ptr;
			++ptr;
			++*ptr;
			while(*ptr)
			{
				ptr -= 4;
			}
			--ptr;
			++*ptr;
			ptr += 2;
			while(*ptr)
			{
				++*ptr;
				--ptr;
			}
			--ptr;
			while(*ptr)
			{
				++ptr;
			}
			++ptr;
			++*ptr;
			while(*ptr)
			{
				while(*ptr)
				{
					ptr += 3;
				}
				ptr += 2;
				++*ptr;
				while(*ptr)
				{
					ptr -= 4;
				}
				++ptr;
				--*ptr;
			}
			++*ptr;
			--ptr;
			++*ptr;
			ptr += 3;
			--*ptr;
			while(*ptr)
			{
				ptr -= 2;
				++*ptr;
				while(*ptr)
				{
					++ptr;
				}
				ptr += 2;
				++*ptr;
				ptr -= 3;
				++*ptr;
				--ptr;
				++*ptr;
				--ptr;
				*ptr -= 8;
				while(*ptr)
				{
					ptr -= 2;
					--*ptr;
					ptr -= 2;
					++*ptr;
					while(*ptr)
					{
						++ptr;
					}
					++ptr;
					++*ptr;
					ptr -= 2;
					--*ptr;
					ptr -= 2;
					--*ptr;
					while(*ptr)
					{
						ptr -= 3;
						++*ptr;
						--ptr;
						--*ptr;
						while(*ptr)
						{
							ptr += 2;
						}
						--ptr;
						--*ptr;
						--ptr;
						--*ptr;
						ptr -= 3;
						--*ptr;
						--ptr;
						*ptr -= 4;
						while(*ptr)
						{
							ptr -= 3;
							--*ptr;
							ptr += 4;
							++*ptr;
							--ptr;
							--*ptr;
							while(*ptr)
							{
								ptr -= 3;
								++*ptr;
								while(*ptr)
								{
									++ptr;
								}
								++ptr;
								++*ptr;
								ptr -= 2;
								++*ptr;
								--ptr;
								--*ptr;
								--ptr;
								--*ptr;
								while(*ptr)
								{
									ptr -= 2;
									++*ptr;
									--ptr;
									--*ptr;
									--ptr;
									++*ptr;
									while(*ptr)
									{
										ptr += 2;
									}
									--ptr;
									++*ptr;
									ptr -= 4;
									++*ptr;
									--ptr;
									--*ptr;
									while(*ptr)
									{
										ptr -= 2;
										--*ptr;
										while(*ptr)
										{
											++ptr;
										}
										ptr += 2;
										--*ptr;
										ptr -= 3;
										--*ptr;
										--ptr;
										--*ptr;
										--ptr;
										--*ptr;
										while(*ptr)
										{
											ptr -= 3;
											++*ptr;
											--ptr;
											--*ptr;
											while(*ptr)
											{
												ptr += 2;
											}
											--ptr;
											++*ptr;
											ptr -= 3;
											++*ptr;
											--ptr;
											++*ptr;
											--ptr;
											--*ptr;
											while(*ptr)
											{
												ptr -= 4;
												++*ptr;
												while(*ptr)
												{
													++ptr;
												}
												--ptr;
												--*ptr;
												ptr -= 2;
												--*ptr;
												while(*ptr)
												{
													ptr -= 2;
													++*ptr;
													while(*ptr)
													{
														++ptr;
													}
													ptr += 2;
													--*ptr;
													ptr -= 4;
													--*ptr;
													--ptr;
													--*ptr;
													while(*ptr)
													{
														ptr += 5;
														++*ptr;
														--ptr;
														--*ptr;
														ptr -= 3;
														++*ptr;
														--ptr;
														--*ptr;
														while(*ptr)
														{
															ptr += 2;
															++*ptr;
															ptr -= 2;
															--*ptr;
															while(*ptr)
															{
																ptr -= 2;
																--*ptr;
																--ptr;
																--*ptr;
																while(*ptr)
																{
																	++ptr;
																}
																++ptr;
																++*ptr;
																ptr -= 2;
																--*ptr;
																--ptr;
																--*ptr;
																--ptr;
																--*ptr;
																while(*ptr)
																{
																	ptr -= 2;
																	++*ptr;
																	--ptr;
																	++*ptr;
																	while(*ptr)
																	{
																		++ptr;
																	}
																	--ptr;
																	++*ptr;
																	--ptr;
																	++*ptr;
																	--ptr;
																	--*ptr;
																	while(*ptr)
																	{
																		ptr += 2;
																		--*ptr;
																		--ptr;
																		--*ptr;
																		--ptr;
																		--*ptr;
																		while(*ptr)
																		{
																			ptr -= 2;
																			--*ptr;
																			while(*ptr)
																			{
																				++ptr;
																			}
																			--ptr;
																			++*ptr;
																			--ptr;
																			*ptr += 4;
																			while(*ptr)
																			{
																				--ptr;
																				*ptr -= 8;
																				++ptr;
																				--*ptr;
																			}
																			*ptr += 2;
																			--ptr;
																			while(*ptr)
																			{
																				ptr -= 2;
																				++*ptr;
																				while(*ptr)
																				{
																					++ptr;
																				}
																				ptr += 2;
																				--*ptr;
																				--ptr;
																				--*ptr;
																				ptr -= 4;
																				--*ptr;
																				while(*ptr)
																				{
																					ptr -= 2;
																					--*ptr;
																					ptr -= 2;
																					--*ptr;
																					ptr += 4;
																					--*ptr;
																					while(*ptr)
																					{
																						ptr -= 4;
																						++*ptr;
																						while(*ptr)
																						{
																							++ptr;
																						}
																						++ptr;
																						++*ptr;
																						ptr -= 4;
																						--*ptr;
																						while(*ptr)
																						{
																							ptr -= 2;
																							++*ptr;
																							ptr -= 2;
																							--*ptr;
																							while(*ptr)
																							{
																								ptr += 2;
																							}
																							--ptr;
																							++*ptr;
																							ptr -= 5;
																							--*ptr;
																							while(*ptr)
																							{
																								ptr += 4;
																								--*ptr;
																								ptr -= 3;
																								--*ptr;
																								--ptr;
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
										}
									}
								}
							}
						}
					}
				}
			}
		}
		++ptr;
		while(*ptr)
		{
			++ptr;
			while(*ptr)
			{
				while(*ptr)
				{
					while(*ptr)
					{
						ptr -= 4;
					}
					++ptr;
					++*ptr;
					ptr += 2;
					while(*ptr)
					{
						ptr += 5;
					}
					--ptr;
					--*ptr;
				}
				--ptr;
			}
			ptr += 3;
			++*ptr;
			ptr += 7;
			++*ptr;
			++ptr;
		}
		--ptr;
	}
	--ptr;
	while(*ptr)
	{
		--*ptr;
	}
	ptr -= 7;
	*ptr += 2;
	--ptr;
	*ptr += 3;
	--ptr;
	*ptr += 3;
	while(*ptr)
	{
		while(*ptr)
		{
			++ptr;
		}
		ptr += 6;
		*ptr += 8;
		while(*ptr)
		{
			ptr -= 2;
			*ptr += 4;
			++ptr;
			*ptr += 6;
			++ptr;
			--*ptr;
		}
		--ptr;
		--*ptr;
		ptr -= 2;
		while(*ptr)
		{
			--*ptr;
			while(*ptr)
			{
				--ptr;
				++*ptr;
				ptr += 2;
				putc(*ptr, out);
				--ptr;
				--*ptr;
			}
		}
		ptr -= 4;
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
			ptr += 5;
			while(*ptr)
			{
				putc(*ptr, out);
				while(*ptr)
				{
					++ptr;
				}
			}
			ptr -= 2;
			while(*ptr)
			{
				--ptr;
				++*ptr;
				++ptr;
				--*ptr;
			}
			ptr += 3;
			while(*ptr)
			{
				ptr -= 2;
				*ptr += 2;
				while(*ptr)
				{
					--ptr;
					++*ptr;
					++ptr;
					*ptr -= 2;
				}
				ptr += 2;
				--*ptr;
			}
			ptr -= 2;
			while(*ptr)
			{
				--*ptr;
				++ptr;
				++*ptr;
				--ptr;
				while(*ptr)
				{
					--ptr;
					*ptr += 2;
					++ptr;
					--*ptr;
				}
			}
			ptr -= 3;
			while(*ptr)
			{
				--ptr;
				++*ptr;
				++ptr;
				--*ptr;
			}
			ptr -= 4;
		}
		ptr += 2;
		++*ptr;
		ptr += 3;
		*ptr -= 2;
		while(*ptr)
		{
			--ptr;
			++*ptr;
			++ptr;
			*ptr -= 3;
		}
		--ptr;
		putc(*ptr, out);
		ptr += 2;
		while(*ptr)
		{
			while(*ptr)
			{
				--*ptr;
			}
			ptr -= 2;
		}
		--ptr;
	}
	while(*ptr)
	{
		--*ptr;
		putc(*ptr, out);
		*ptr = getc(in);
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
