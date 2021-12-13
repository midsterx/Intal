#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intal.h"

typedef struct intarl
{
	char* s;
	int n;
} intarl;

void removePadding(intarl* intal)
{
	int i = intal->n-1;
	while(i>0 && intal->s[i]==0)
		i--;
	intal->n = i+1;
}

intarl** splitFunc(intarl* a, int m)
{
	int i,n;
	intarl *intal1 = (intarl*)a;
	intarl *a1 = (intarl*)malloc(sizeof(intarl));
	intarl *a2 = (intarl*)malloc(sizeof(intarl));
	intarl **arr = (intarl**)malloc(sizeof(intarl*)*2);
	a1->n = a2->n = m;
	a1->s = intal1->s + m;
	a2->s = intal1->s;
	*(arr+0) = a2;
	*(arr+1) = a1;
	return arr;
}

void* intal_create(const char* str)
{
	int i,k,j = 0,h = 0;
	char c;
	int n = strlen(str);
	if(str == NULL)
	{
		return NULL; //invalid string
	}
	intarl* num = (intarl*)malloc(sizeof(intarl));
	while(j < n-1 && str[j] == '0')
	{
		j++;
	}
	for (i = 0;str[i]!='\0';i++)
	{
		if(str[i]-'0' > 9 || str[i]-'0' < 0)
		{
			h++;
		}
	}
	if (h == n)
		return NULL;
	k = j;
	num->s = (char*)malloc(n-j-h+1);
	for (i = n-k-h-1;i >= 0;j++) // Little Endian Representation
	{
		c = str[j] - '0';
		if(c > 9 || c < 0)
			j++;
		else
			num->s[i--] = str[j] - '0';
	}
	num->s[n-k-h] = '\0';
	num->n = n-k-h;
	return num;
}

void intal_destroy(void* intal)
{
	intarl* i = (intarl*)intal;
	if (i == NULL)
		return;
	free(i->s);
	free(i);
}

char* intal2str(void* intal)
{
	intarl* num = (intarl*)intal;
	char *number;
	if (num == NULL)
	{
		number = (char*)malloc(4*sizeof(char));
		number[0] = 'N';
		number[1] = 'a';
		number[2] = 'N';
		number[3] = '\0';
		return number;
	}

	int i,j,n = num->n;
	number = (char*)malloc(n+1);
	for(i = 0,j = n-1;i < n,j >= 0;i++,j--)
		number[j] = num->s[i] + '0';
	number[n] = '\0';
	return number;
}

void* intal_increment(void* intal)
{
	intarl *num = (intarl*)intal;
	if(num == NULL)
		return NULL;
	intarl *res = (intarl*)malloc(sizeof(intarl));
	res->s = (char*)malloc(num->n+1);
	int i,n,carry;
	n = num->n;
	i = 0;
	do
	{
		res->s[i] = num->s[i]+1;
		carry = res->s[i] > 9 ? 1 : 0;
		res->s[i] = carry == 0 ? res->s[i] : 0;
		i++;
	} while(i < n && carry != 0);
	while(i < n)
	{
		res->s[i] = num->s[i];
		i++;
	}
	if(carry == 1)
	{
		res->s[i] = 1;
		n++;
	}
	free(num->s);
	free(num);
	res->n = n;
	return res;
}

void* intal_decrement(void* intal)
{
	intarl *num = (intarl*)intal;
	if(num == NULL)
		return NULL;
	intarl *res = (intarl*)malloc(sizeof(intarl));
	int i,n,borrow;
	n = num->n;
	i = 0;
	if(n==1 && (num->s[0]==0 || num->s[0] == 1))
	{
		res->s = (char*)malloc(1);
		res->n = 1;
		res->s[0] = 0;
		free(num->s);
		free(num);
		return res;
	}
	res->s = (char*)malloc(n+1);
	while(num->s[i]==0 && i<n)
	{
		res->s[i] = 9;
		i++;
	}
	res->s[i] = num->s[i]-1;
	i++;
	while(i < n)
	{
		res->s[i] = num->s[i];
		i++;
	}
	if(res->s[i-1] == 0)
	{
		n--;
	}
	res->n = n;
	free(num->s);
	free(num);
	return res;
}

void* intal_add(void* intal1, void* intal2)
{
	intarl *a = (intarl*)intal1;
	intarl *b = (intarl*)intal2;
	if(a==NULL || b==NULL)
		return NULL;
	int n,len,carry = 0,i = 0;
	intarl *sum = (intarl*)malloc(sizeof(intarl));
	len = a->n > b->n ? a->n : b->n; // Larger of the two numbers is taken as len
	sum->s = (char*)malloc(len+2);
	n = a->n < b->n ? a->n : b->n; // Smaller of the two numbers is taken as n
	while(i < n)
	{
		sum->s[i] = a->s[i] + b->s[i] + carry;
		carry = sum->s[i] > 9 ? sum->s[i]/10 : 0;
		sum->s[i] = sum->s[i] % 10;
		i++;
	}
	while(i < a->n)
	{
		sum->s[i] = a->s[i] + carry;
		carry = sum->s[i] > 9 ? sum->s[i]/10 : 0;
		sum->s[i] = sum->s[i] % 10;
		i++;
	}
	while(i < b->n)
	{
		sum->s[i] = b->s[i] + carry;
		carry = sum->s[i] > 9 ? sum->s[i]/10 : 0;
		sum->s[i] = sum->s[i] % 10;
		i++;
	}
	if(carry > 0)
	{
		sum->s[i] = carry;
		i++;
	}
	sum->n = i;
	return sum;
}

void* intal_diff(void* intal1, void* intal2)
{
	intarl *a = (intarl*)intal1;
	intarl *b = (intarl*)intal2;
	if(a == NULL || b == NULL)
		return NULL;
	int i,t,sub,borrow;
	intarl *big,*small,*diff;
	int nb,ns;
	diff = (intarl*)malloc(sizeof(intarl));
	t = intal_compare(a,b);
	if(t == 0) // a == b - 0
	{
		diff->n = 1;
		diff->s = (char*)malloc(1);
		diff->s[0] = 0;
		return diff;
	}
	else if(t == 1) // a > b - Continue
	{
		big = a;
		small = b;
	}
	else // b > a - Switch
	{
		big = b;
		small = a;
	}
	nb = big->n;
	ns = small->n;
	i = 0;
	diff->s = (char*)malloc(nb+1);
	diff->s[0] = 0;
	while(i < ns)
	{
		sub = big->s[i] - small->s[i];
		if(sub+diff->s[i] < 0)
		{
			diff->s[i] += sub+10;
			diff->s[i+1] = -1;
		}
		else
		{
			diff->s[i] += sub;
			diff->s[i+1] = 0;
		}
		i++;
	}
	while(i < nb)
	{
		diff->s[i] += big->s[i];
		if(diff->s[i] < 0)
		{
			diff->s[i] = 9;
			diff->s[i+1] = -1;
		}
		else
			diff->s[i+1] = 0;
		i++;
	}
	while(i>0 && diff->s[i]==0)
	{
		i--;
	}
	diff->n = i+1;
	return diff;
}

int intal_compare(void* intal1, void* intal2)
{
	int i = 0;
	intarl* a = (intarl*)intal1;
	intarl* b = (intarl*)intal2;
	if (a == NULL || b == NULL)
		return -2;
	if (a->n > b->n)
		return 1;
	else if (a->n < b->n)
		return -1;
	else
	{
		for(i = a->n - 1;i >= 0;i--)
		{
			if (a->s[i] > b->s[i])
				return 1;
			if (b->s[i] > a->s[i])
				return -1;
		}
	}
	return 0;
}

intarl* karatsuba(intarl* a, intarl* b)
{
	int k,m,i,j,n;	// m -> Split Point
	int flag = 0;
	char prod,old1 = 0,old2 = 0;

	intarl *a1,*a2,*b1,*b2;
	intarl **arr1,**arr2;
	intarl *tempA,*tempB;
	intarl *e1,*e2;	// 10^n (Exponents 1 and 2)

	intarl *res,*p1,*p2,*p3,*sumA,*sumB;
	intarl *diff31, *diff312;	// To perform p3-p2-p1

	intarl *add1, *add2;	// To add p1 and p2 respectively

	intarl *intal1 = (intarl*)a;
	intarl *intal2 = (intarl*)b;
	i = intal1->n;
	while(i < intal2->n)
	{
		intal1->s[i] = 0;
		i++;
	}
	intal1->n = i;
	i = intal2->n;
	while(i < intal1->n)
	{
		intal2->s[i] = 0;
		i++;
	}
	intal2->n = i;
	n = intal1->n;
	res = (intarl*)malloc(sizeof(intarl));
	res->s = (char*)malloc(2*n+1);
	if(n == 1)
	{
		prod = intal1->s[0]*intal2->s[0];
		if(prod > 9)
		{
			res->s[0] = prod%10;
			res->s[1] = prod/10;
			res->n = 2;
			return res;
		}
		res->s[0] = prod;
		res->n = 1;
		return res;
	}
	// To make it faster, repeat the base case for n = 2 to n = 8
	else if(n%2 == 1)
	{
		old1 = intal1->s[n];
		intal1->s[n] = 0;
		intal1->n++;
		old2 = intal2->s[n];
		intal2->s[n] = 0;
		intal2->n++;
		n++;
		flag = 1;
	}
	m = n/2;
	arr1 = splitFunc(intal1, m);
	arr2 = splitFunc(intal2, m);

	a1 = arr1[1];
	a2 = arr1[0];
	b1 = arr2[1];
	b2 = arr2[0];

	p1 = karatsuba(a1,b1);
	p2 = karatsuba(a2,b2);

	tempA = (intarl*)intal_add((void*)a1,(void*)a2);
	tempB = (intarl*)intal_add((void*)b1,(void*)b2);

	int x,glen;

	glen = tempA->n > tempB->n ? tempA->n : tempB->n;

	sumA = (intarl*)malloc(sizeof(intarl));
	sumA->s = (char*)malloc(2*glen);
	for (x = 0;x < tempA->n;x++)
		sumA->s[x] = tempA->s[x];
	sumA->n = tempA->n;

	sumB = (intarl*)malloc(sizeof(intarl));
	sumB->s = (char*)malloc(2*glen);
	for (x = 0;x < tempB->n;x++)
		sumB->s[x] = tempB->s[x];
	sumB->n = tempB->n;

	p3 = karatsuba(sumA,sumB);

	free(a1);	// Memory Free
	free(a2);
	free(b1);
	free(b2);
	free(arr1);
	free(arr2);
	intal_destroy(tempA);
	intal_destroy(tempB);
	intal_destroy(sumA);
	intal_destroy(sumB);

	removePadding(p1);
	removePadding(p2);
	removePadding(p3);

	diff31 = (intarl*)intal_diff((void*)p3,(void*)p1);

	diff312 = (intarl*)intal_diff((void*)diff31,(void*)p2);
	e2 = (intarl*)malloc(sizeof(intarl));
	e2->n = p2->n > m ? p2->n - m : 0;
	e2->s = p2->s + m;
	add2 = (intarl*)intal_add((void*)e2,(void*)diff312);

	e1 = (intarl*)malloc(sizeof(intarl));
	e1->n = add2->n > m ? add2->n - m: 0;
	e1->s = add2->s + m;
	add1 = (intarl*)intal_add((void*) e1,(void*) p1);

	for(i=0;i<m && i< p2->n;i++)
		res->s[i] = p2->s[i];
	for(;i<m;i++)
		res->s[i] = 0;
	for(j=0;i<n && j<add2->n;i++,j++)
		res->s[i] = add2->s[j];
	for(;i<n;i++)
		res->s[i] = 0;
	for(j=0;j < add1->n;i++,j++)
		res->s[i] = add1->s[j];
	res->n = i;
	removePadding(res);
	intal_destroy(diff31);
	intal_destroy(diff312);

	intal_destroy(add1);
	intal_destroy(add2);
	intal_destroy(p1);
	intal_destroy(p2);
	intal_destroy(p3);
	free(e1);
	free(e2);

	if(flag==1)
	{
		n--;
		intal1->s[n] = old1;
		intal2->s[n] = old2;
		intal1->n--;
		intal2->n--;
	}
	return res;
}

void* intal_multiply(void* intal1, void* intal2)
{
	int i;
	intarl *a = (intarl*)intal1;
	intarl *b = (intarl*)intal2;
	if (a == NULL || b == NULL)
		return NULL;
	intarl *var1 = (intarl*)malloc(sizeof(intarl));
	intarl *var2 = (intarl*)malloc(sizeof(intarl));
	intarl *res;
	int len = a->n > b->n ? a->n : b->n;
	len = len%2 == 0 ? len: len + 1;
	var1->s = (char*)malloc(2*len);
	var2->s = (char*)malloc(2*len);
	for(i = 0;i < a->n;i++)
		var1->s[i] = a->s[i];
	for(;i < len;i++)		// Zero Padding
		var1->s[i] = 0;
	var1->n = len;
	for(i=0;i < b->n;i++)
		var2->s[i] = b->s[i];
	for(;i < len;i++)		// Zero Padding
		var2->s[i] = 0;
	var2->n = len;
	res = karatsuba(var1, var2);
	intal_destroy(var1);
	intal_destroy(var2);
	return res;
}

void* intal_divide(void* intal1, void* intal2)
{
	int cmp,n1,n2,i,j;
	intarl *a = (intarl*)intal1;
	intarl *b = (intarl*)intal2;
	if(a == NULL || b == NULL)
		return NULL;
	if(b->n == 1 && b->s[0] == 0)
		return NULL;
	intarl *substr = (intarl*)malloc(sizeof(intarl));
	intarl *remainder;
	intarl *temp;
	intarl *buffer;
	intarl *res = (intarl*)malloc(sizeof(intarl));	// Quotient
	if(b->n == 1 && b->s[0] == 1)
	{
		res->s = (char*)malloc(a->n+1);
		res->n = a->n;
		for (i = 0;i < a->n;i++)
			res->s[i] = a->s[i];
		return res;
	}
	cmp = intal_compare((void*)a,(void*)b);	//Comparing the two numbers
	if(cmp == 0)
	{
		res->s = (char*)malloc(2);
		res->s[0] = 1;
		res->n = 1;
		return res;
	}
	else if (cmp == -1)
	{
		res->s = (char*)malloc(2);
		res->s[0] = 0;
		res->n = 1;
		return res;
	}
	n1 = a->n;
	n2 = b->n;
	buffer = (intarl*)malloc(sizeof(intarl));
	buffer->s = (char*)malloc(a->n+1);
	res->s = (char*)malloc(a->n+1);
	res->n = a->n;
	for(i = 0;i < n1;i++)
		buffer->s[i] = a->s[i];
	buffer->n = n1;
	substr->s = buffer->s + n1 - 1;
	substr->n = 1;
	i = n1 - 1;
	while(i>=0)
	{
		removePadding(substr);
		for(;i>=0 && substr->n<n2;i--)
		{
			substr->s--;
			substr->n++;
			res->s[i] = 0;
			removePadding(substr);
		}
		while(intal_compare((void*)substr, (void*)b)==-1 && i>=0)
		{
			substr->s--;
			substr->n++;
			res->s[i] = 0;
			i=i-1;
			removePadding(substr);
		}
		if(i<0)
		{
			break;
		}
		remainder = intal_diff((void*)substr, (void*)b);
		res->s[i] = 1;
		while(intal_compare((void*)remainder, (void*)b) != -1)
		{
			temp = intal_diff((void*)remainder, (void*)b);
			intal_destroy(remainder);
			remainder = temp;
			res->s[i] = res->s[i] + 1;
		}
		for(j = 0;j < remainder->n;j++)
		{
			substr->s[j] = remainder->s[j];
		}
		substr->s--;	// Moving to the next number
		substr->n = j+1;
		intal_destroy(remainder);
		i--;
	}
	intal_destroy(buffer);
	free(substr);
	removePadding(res);
	return res;
}

void* intal_pow(void* intal1, void* intal2)
{
	intarl *a = (intarl*)intal1;
	intarl *b = (intarl*)intal2;
	intarl *res = NULL;
	intarl *r1 = NULL;
	intarl *r2 = NULL;
	intarl *two = NULL;
	intarl *half = NULL;
	if (a == NULL || b == NULL)
		return NULL;
	if (a->n == 1 && b->n == 1 && a->s[0] == 0 && b->s[0] == 0)
	{
		res = (intarl*)malloc(sizeof(intarl));
		res->s = (char*)malloc(2);
		res->s[0] = 0;
		res->n = 1;
		return res;
	}
	if(a->n == 1 && a->s[0] == 0)
	{
		res = (intarl*)malloc(sizeof(intarl));
		res->s = (char*)malloc(2);
		res->s[0] = 0;
		res->n = 1;
		return res;
	}
	if(b->n == 1 && b->s[0] == 0)
	{
		res = (intarl*)malloc(sizeof(intarl));
		res->s = (char*)malloc(2);
		res->s[0] = 1;
		res->n = 1;
		return res;
	}

	two = (intarl*)malloc(sizeof(intarl));	// 2
	two->s = (char*)malloc(2);
	two->s[0] = 2;
	two->n = 1;

	half = intal_divide(b, two); // b/2

	res = intal_pow((void*)a, (void*)half);

	r1 = intal_multiply((void*)res, (void*)res);
	if(b->s[0]%2 == 1)
	{
		r2 = intal_multiply(r1, a);
	}
	intal_destroy(res);
	intal_destroy(two);
	intal_destroy(half);
	if(r2 != NULL)
	{
		res = r2;
		intal_destroy(r1);
	}
	else
	{
		res = r1;
		intal_destroy(r2);
	}
	return res;
}
