/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 2 - proj2.c
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#define MIN_NUM_PROVIDERS 1
#define MIN_NUM_SUPPLY_ST 0
#define MIN_NUM_CONNECTIONS 0

int main()
{
	int F;   /* Number of providers */
	int E;   /* Number of supply stations */
	int T;   /* Number of network connections */

	if (!scanf("%d %d %d", &F, &E, &T) || F < MIN_NUM_PROVIDERS || E < MIN_NUM_SUPPLY_ST || T < MIN_NUM_CONNECTIONS)
	{
		printf("Invalid input!\n");
		exit(1);
	}

	/* (...) */
	
	return 0;
}