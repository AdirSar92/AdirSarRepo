#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /* strlen */
#include <stdio.h>	/* sprintf */
#include <stdint.h> /* UINT32_MAX */

#include "dhcp.h"

#define NETWORK_ADDRESS (0)
#define SERVER_ADDRESS (254)
#define BROADCAST_ADDRESS (255)

#define FULL (1)
#define NOT_FULL (0)
#define AVAILABLE (-1)

#define OCTET (8)
#define FAIL (1)
#define SUCCESS (0)
#define IP_SIZE (32)
#define OFFSET_SHIFT (IP_SIZE - 1)
#define PARENT (1)

typedef enum children
{
	ZERO = 0,
	ONE,
	NUM_OF_CHILDREN
} children_t;

typedef struct dhcp_node dhcp_node_t;

struct dhcp
{
	dhcp_node_t *root;
	uint32_t subnet_ID;
	size_t mask_length;
};

struct dhcp_node
{
	dhcp_node_t *children[NUM_OF_CHILDREN];
	int is_full;
};

static void DestroyNodes(dhcp_node_t *curr_node);
static int AllocateRec(dhcp_node_t *node, uint32_t requested_ip, uint32_t *result_ip, int index);
static void UpdateFullness(dhcp_node_t *node);
static dhcp_node_t *GoToSubnetRoot(dhcp_t *dhcp);
static dhcp_node_t *FreeIpRec(dhcp_node_t *curr_node, uint32_t ip);
static size_t CountLeaves(dhcp_node_t *curr_node, int bit);
static dhcp_node_t *CreateNode(int is_full);
static int CreateSubnetID(dhcp_t *dhcp);
static children_t DetermineDirection(uint32_t index, int bit);
static uint32_t GetFreeIp(uint32_t requested_ip, dhcp_node_t *node, int index);
static int IsIpAddressFree(uint32_t requested_ip, dhcp_node_t *node, int index);
static int IsIpValid(const dhcp_t *dhcp, uint32_t requested_ip);

dhcp_t *DHCPCreate(uint32_t subnet_ID, size_t subnet_length)
{
	dhcp_t *dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
	if (NULL != dhcp)
	{
		dhcp->subnet_ID = subnet_ID;
		dhcp->mask_length = subnet_length;
		dhcp->root = CreateNode(NOT_FULL);
		if (NULL == dhcp->root)
		{
			free(dhcp);
			return (NULL);
		}
		if (FAIL == CreateSubnetID(dhcp))
		{
			DHCPDestroy(dhcp);
			return (NULL);
		}
	}
	return (dhcp);
}

void DHCPDestroy(dhcp_t *dhcp)
{
	assert(NULL != dhcp);
	DestroyNodes(dhcp->root);
	free(dhcp);
}

static void DestroyNodes(dhcp_node_t *curr_node)
{
	if (NULL == curr_node)
	{
		return;
	}

	DestroyNodes(curr_node->children[ZERO]);
	curr_node->children[ZERO] = NULL;
	DestroyNodes(curr_node->children[ONE]);
	curr_node->children[ONE] = NULL;

	free(curr_node);
}

int DHCPAllocateIp(dhcp_t *dhcp, uint32_t requested_ip, uint32_t *result_ip)
{
	dhcp_node_t *subnet_root = NULL;

	assert(NULL != dhcp);
	assert(NULL != result_ip);

	subnet_root = GoToSubnetRoot(dhcp);
	*result_ip = dhcp->subnet_ID;
	requested_ip = GetFreeIp(requested_ip, subnet_root, (IP_SIZE - dhcp->mask_length));
	if (FAIL == requested_ip || !IsIpValid(dhcp, requested_ip))
	{
		return (FAIL);
	}

	return (AllocateRec(subnet_root, requested_ip, result_ip, (OFFSET_SHIFT - dhcp->mask_length)));
}

static int AllocateRec(dhcp_node_t *node, uint32_t requested_ip, uint32_t *result_ip, int index)
{
	uint32_t side = 0;

	if (index >= 0)
	{
		side = DetermineDirection(requested_ip, index);
		if (NULL == node->children[side])
		{
			node->children[side] = (CreateNode(0 == index));
			if (NULL == node->children[side])
			{
				return (FAIL);
			}
		}

		*result_ip |= (side << index);

		if (FAIL == AllocateRec(node->children[side], requested_ip, result_ip, index - 1))
		{
			return (FAIL);
		}
	}

	if (NULL != node)
	{
		UpdateFullness(node);
	}
	return (SUCCESS);
}

static void UpdateFullness(dhcp_node_t *node)
{
	if (NULL == node->children[ZERO] || NULL == node->children[ONE])
	{
		return;
	}
	node->is_full = (node->children[ZERO]->is_full & node->children[ONE]->is_full);
}

static dhcp_node_t *GoToSubnetRoot(dhcp_t *dhcp)
{
	dhcp_node_t *node = NULL;
	uint32_t mask;
	size_t mask_len;

	assert(NULL != dhcp);

	node = dhcp->root;
	mask = dhcp->subnet_ID;
	mask_len = dhcp->mask_length;

	while (0 < mask_len)
	{
		node = node->children[DetermineDirection(mask, OFFSET_SHIFT)];
		mask <<= 1;
		--mask_len;
	}

	return (node);
}

void DHCPFreeIp(dhcp_t *dhcp, uint32_t ip)
{
	assert(NULL != dhcp);
	dhcp->root = FreeIpRec(dhcp->root, ip);
}

static dhcp_node_t *FreeIpRec(dhcp_node_t *curr_node, uint32_t ip)
{
	children_t side = DetermineDirection(ip, OFFSET_SHIFT);
	if (NULL != curr_node)
	{
		curr_node->children[side] = FreeIpRec(curr_node->children[side], ip << 1);
		if (NULL == curr_node->children[ZERO] && NULL == curr_node->children[ONE])
		{
			free(curr_node);
			curr_node = NULL;
			return (NULL);
		}
		UpdateFullness(curr_node);
	}
	return (curr_node);
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
	uint32_t max = UINT32_MAX;
	assert(NULL != dhcp);

	return ((max / (1 << dhcp->mask_length)) - CountLeaves(dhcp->root, 0) + 1);
}

static size_t CountLeaves(dhcp_node_t *curr_node, int bit)
{
	if (NULL != curr_node)
	{
		if (FULL == curr_node->is_full)
		{
			return (1 << (IP_SIZE - bit));
		}
		return (CountLeaves(curr_node->children[ZERO], bit + 1) +
				CountLeaves(curr_node->children[ONE], bit + 1));
	}
	return (0);
}

char *DHCPIpToString(uint32_t ip, char *dest_ip)
{
	int count = 3;
	unsigned char temp;
	char *to_return = dest_ip;

	while (0 <= count)
	{
		temp = (ip >> (count * OCTET)) & 0xFF;
		sprintf(dest_ip + strlen(dest_ip), "%d.", temp);
		--count;
	}

	dest_ip[strlen(dest_ip) - 1] = '\0';

	return (to_return);
}

uint32_t DHCPStringToIp(char *ip)
{
	uint32_t ret_ip = 0;
	while ('\0' != *ip)
	{
		ret_ip <<= OCTET;
		ret_ip |= strtol(ip, &ip, 10);
		if ('\0' != *ip)
		{
			++ip;
		}
	}

	return (ret_ip);
}

static dhcp_node_t *CreateNode(int is_full)
{
	dhcp_node_t *new_node = (dhcp_node_t *)malloc(sizeof(dhcp_node_t));
	if (NULL != new_node)
	{
		new_node->children[ZERO] = NULL;
		new_node->children[ONE] = NULL;
		new_node->is_full = is_full;
	}
	return (new_node);
}

static int CreateSubnetID(dhcp_t *dhcp)
{
	dhcp_node_t *node = dhcp->root;
	uint32_t mask = dhcp->subnet_ID, temp = 0;
	size_t mask_len = dhcp->mask_length;
	children_t side;

	while (0 < mask_len)
	{
		side = DetermineDirection(mask, OFFSET_SHIFT);
		node->children[side] = CreateNode(0);
		if (NULL == node->children[side])
		{
			return (FAIL);
		}
		node = node->children[side];
		mask <<= 1;
		--mask_len;
	}

	DHCPAllocateIp(dhcp, dhcp->subnet_ID | NETWORK_ADDRESS, &temp);
	DHCPAllocateIp(dhcp, dhcp->subnet_ID | SERVER_ADDRESS, &temp);
	DHCPAllocateIp(dhcp, dhcp->subnet_ID | BROADCAST_ADDRESS, &temp);

	return (SUCCESS);
}

static children_t DetermineDirection(uint32_t ip, int bit)
{
	return (!!(ip & (1 << bit)));
}

static uint32_t GetFreeIp(uint32_t requested_ip, dhcp_node_t *node, int index)
{
	int curr_bit = 0;
	do
	{
		curr_bit = IsIpAddressFree(requested_ip, node, index);
		if (curr_bit == index)
		{
			return (FAIL);
		}
		if (curr_bit != AVAILABLE)
		{
			requested_ip += (1 << curr_bit);
		}
	} while (AVAILABLE != curr_bit);

	return (requested_ip);
}

static int IsIpAddressFree(uint32_t requested_ip, dhcp_node_t *node, int index)
{
	while (0 < index--)
	{
		node = node->children[DetermineDirection(requested_ip, index)];
		if (NULL == node)
		{
			return (AVAILABLE);
		}
		if (FULL == node->is_full)
		{
			return (index);
		}
	}
	return (FAIL);
}

static int IsIpValid(const dhcp_t *dhcp, uint32_t requested_ip)
{
	uint32_t max = UINT32_MAX;
	return (0 == ((requested_ip ^ dhcp->subnet_ID) & (max << (IP_SIZE - dhcp->mask_length))));
}