
enum {
	Port4MCP		= 0x80060008,
	Port4SSP		= 0x8007006c,
	AudioDMA		= 0xa,
	SSPXmitDMA		= 0xe,
	SSPRecvDMA		= 0xf,
};

void	dmainit(void);
int		dmaalloc(int, int, int, int, int, ulong, void (*)(void*, ulong), void*);
void	dmafree(int);
ulong	dmastart(int, void *, int);
ulong	dmadone(int, ulong);
void	dmawait(int, ulong);
int		dmaidle(int chan);
