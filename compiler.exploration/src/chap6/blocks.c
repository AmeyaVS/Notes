#include <stdio.h>

int main()
{
	/* Block B0 Begins */

	int v1=10,v2=20;
	int v3=5;

	{
		/* Block B1 Begins */
		int v1=20,v2=30;

		{
			/* Block B2 Begins */
			int v1=40,v2=50;

			v3=90;
			printf("In Block B2 v1=%d v2=%d v3=%d \n",v1,v2,v3);

			/* Block B2 ends */
		}

		{
			/* Block B3 Begins */
			int v1=60,v2=70;

			v3=80;
			printf("In Block B3 v1=%d v2=%d v3=%d \n",v1,v2,v3);

			/* Block B3 ends */
		}
		printf("In Block B1 v1=%d v2=%d v3=%d \n",v1,v2,v3);

		/* Block B1 ends */
	}

	printf("In Block B0 v1=%d v2=%d v3=%d \n",v1,v2,v3);

	return(0);

	/* Block B0 ends */
}

