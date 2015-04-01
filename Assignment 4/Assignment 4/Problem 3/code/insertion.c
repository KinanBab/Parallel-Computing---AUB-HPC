// Perform insertion sort on a list a of size n
void insertion_sort(int a[], int n);


void insertion_sort(int a[], int n)
{
	int i, hold, marker;
	for(i = 0; i < n; i++)
	{
		hold = a[i];
		marker = i;
		while(marker > 0 && hold < a[marker - 1])
		{
			a[marker] = a[marker - 1];
			marker--;
		}
		a[marker] = hold;
	}
}
