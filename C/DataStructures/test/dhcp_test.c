#include <stdio.h>  /* printf */
#include <string.h> /* strlen */

#include "dhcp.h" /* DHCP */

static void TestAllFuncs();
static void TestCreate();
static void TestDestroy();
static void TestAllocate();
static void TestFree();
static void TestCount();
static void TestStringIpConversion();

int main()
{
    TestAllFuncs();
    return (0);
}

static void TestAllFuncs()
{
    printf("     ~START OF TEST FUNCTION~ \n");
    TestCreate();
    TestCount();
    TestStringIpConversion();
    TestAllocate();
    TestFree();
    TestDestroy();
    printf("      ~END OF TEST FUNCTION~ \n");
}

static void TestCreate()
{
    dhcp_t *dhcp = DHCPCreate(DHCPStringToIp("192.188.0.0"), 16);
    printf("\033[1;31m");
    if (NULL != dhcp)
    {
        printf("DHCPCreate working!                                  V\n");
    }
    else
    {
        printf("DHCPCreate NOT working!                              X\n");
    }

    DHCPDestroy(dhcp);
}

static void TestDestroy()
{
    printf("\033[1;32m");
    printf("*Run vlg to test DHCPDestroy*\n");
}

static void TestStringIpConversion()
{
    char *str = "192.168.0.220";
    char str2[50] = {0};
    uint32_t ip = DHCPStringToIp(str);

    DHCPIpToString(ip, str2);

    printf("\033[1;33m");
    if (0 == strcmp(str, str2))
    {
        printf("DHCPStringToIp & DHCPIpToString working!             V\n");
    }
    else
    {
        printf("DHCPStringToIp & DHCPIpToString NOT working!         X\n");
    }
}

static void TestCount()
{
    dhcp_t *dhcp = DHCPCreate(DHCPStringToIp("192.188.0.0"), 16);
    dhcp_t *dhcp2 = DHCPCreate(DHCPStringToIp("192.168.167.0"), 24);

    printf("\033[1;34m");
    if (65533 == DHCPCountFree(dhcp) && 253 == DHCPCountFree(dhcp2))
    {
        printf("DHCPCountFree working!                               V\n");
    }
    else
    {
        printf("DHCPCountFree NOT working!                           X\n");
    }

    DHCPDestroy(dhcp);
    DHCPDestroy(dhcp2);
}

static void TestAllocate()
{
    uint32_t new_ip = 0;
    uint32_t new_ip2 = 0;
    uint32_t new_ip3 = 0;
    uint32_t new_ip4 = 0;
    uint32_t new_ip5 = 0;
    uint32_t new_ip6 = 0;

    dhcp_t *dhcp = DHCPCreate(DHCPStringToIp("192.168.0.0"), 16);
    DHCPAllocateIp(dhcp, DHCPStringToIp("192.168.0.0"), &new_ip);
    DHCPAllocateIp(dhcp, DHCPStringToIp("192.168.0.0"), &new_ip2);
    DHCPAllocateIp(dhcp, DHCPStringToIp("192.168.0.0"), &new_ip3);
    DHCPAllocateIp(dhcp, DHCPStringToIp("192.168.0.0"), &new_ip4);
    DHCPAllocateIp(dhcp, DHCPStringToIp("192.168.0.0"), &new_ip5);
    DHCPAllocateIp(dhcp, DHCPStringToIp("192.168.0.0"), &new_ip6);

    printf("\033[1;35m");

    if (3232235521 == new_ip && 3232235522 == new_ip2 && 3232235523 == new_ip3 &&
        3232235524 == new_ip4 && 3232235525 == new_ip5 && 3232235526 == new_ip6)
    {
        printf("DHCPAllocateIp working!                              V\n");
    }
    else
    {
        printf("DHCPAllocateIp NOT working!                          X\n");
    }
    DHCPDestroy(dhcp);
}

static void TestFree()
{
    uint32_t new_ip = 0;
    uint32_t new_ip2 = 0;
    size_t free_before = -1, free_after = -1;
    dhcp_t *dhcp = DHCPCreate(DHCPStringToIp("192.188.0.0"), 16);

    DHCPAllocateIp(dhcp, DHCPStringToIp("192.188.1.1"), &new_ip);
    DHCPAllocateIp(dhcp, DHCPStringToIp("192.188.1.1"), &new_ip2);
    free_before = DHCPCountFree(dhcp);
    DHCPFreeIp(dhcp, new_ip);
    free_after = DHCPCountFree(dhcp);

    printf("\033[1;36m");
    if (3233546497 == new_ip && 3233546498 == new_ip2 && (1 == free_after - free_before))
    {
        printf("DHCPFreeIp working!                                  V\n");
    }
    else
    {
        printf("DHCPFreeIp NOT working!                              X\n");
    }
    DHCPDestroy(dhcp);
}
