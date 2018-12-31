/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nature.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define EVENT			1	// �ƥ�
#define CURRENT_WEATHER		2	// �ثe�Ѯ�
#define CURRENT_DISASTER	3	// �ثe�a��
#define WEATHER_QUEUE		4	// �Ѯ�Ƨ�
#define DISASTER_QUEUE		5	// �a���Ƨ�
#define TEMPERATURE		6	// �ū�
#define HUMIDITY		7	// �ë�
#define WIND			8	// ���t
#define NEXTTIME_ARRANGE	9	// �Ƨ�

// �u�`�w�q, �N�Ůt�Ԥj�Φ����A�����ū��ܤ�
#define SEASON_ELEMENT			0
#define SEASON_TEMPERATURE_LOW		1
#define SEASON_TEMPERATURE_HIGH		2
#define SEASON_SUNRISE_HOUR		3
#define SEASON_SUNSET_HOUR		4

#define SEASON_DATA				\
([						\
	SPRING	:	({ WE_SPRING, 14, 28, 6, 18 }),	\
	SUMMER	:	({ WE_SUMMER, 22, 38, 5, 19 }),	\
	AUTUMN	:	({ WE_AUTUMN,  8, 24, 6, 18 }),	\
	WINTER	:	({ WE_WINTER, -2 ,16, 7, 17 }),	\
])

// �Ѯ�|����
#define WE_SPRING		(1<<0)  // �u�`
#define WE_SUMMER		(1<<1)
#define WE_AUTUMN		(1<<2)
#define WE_WINTER		(1<<3)
#define WE_HUMIDITY_0		(1<<4)	// �ë�
#define WE_HUMIDITY_1		(1<<5)
#define WE_HUMIDITY_2		(1<<6)
#define WE_HUMIDITY_3		(1<<7)
#define WE_WIND_0		(1<<8)	// ���t
#define WE_WIND_1		(1<<9)
#define WE_WIND_2		(1<<10)
#define WE_WIND_3		(1<<11)
#define WE_TEMPERATURE_0	(1<<12)	// �ū�
#define WE_TEMPERATURE_1	(1<<13)
#define WE_TEMPERATURE_2	(1<<14)
#define WE_TEMPERATURE_3	(1<<15)
#define WE_ALL_SEASON		(WE_SPRING | WE_SUMMER | WE_AUTUMN | WE_WINTER)
#define WE_ALL_HUMIDITY		(WE_HUMIDITY_0 | WE_HUMIDITY_1 | WE_HUMIDITY_2 | WE_HUMIDITY_3)
#define WE_ALL_WIND		(WE_WIND_0 | WE_WIND_1 | WE_WIND_2 | WE_WIND_3)
#define WE_ALL_TEMPERATURE	(WE_TEMPERATURE_0 | WE_TEMPERATURE_1 | WE_TEMPERATURE_2 | WE_TEMPERATURE_3)

// �ū׼h��
#define TEMPERATURE_LV_0	-2..6
#define TEMPERATURE_LV_1	7..16
#define TEMPERATURE_LV_2	17..26
#define TEMPERATURE_LV_3	27..38


// �a�`���A�w�q
#define DISASTER_CHINESE	0
#define DISASTER_ID		1
#define DISASTER_OCCUR_TERM	2
#define DISASTER_OCCUR_PROB	3

// �U�بa��
#define DISASTER_ARIDITY	({ "����", ARIDITY, WE_HUMIDITY_0 | WE_HUMIDITY_1 | WE_TEMPERATURE_3 | WE_ALL_WIND, 50 })
#define DISASTER_FLOOD		({ "�x��", FLOOD, WE_HUMIDITY_3 | WE_ALL_TEMPERATURE | WE_ALL_WIND, 50 })
#define DISASTER_COLD		({ "�H�y", COLD, WE_HUMIDITY_0 | WE_HUMIDITY_1 | WE_TEMPERATURE_0 | WE_ALL_WIND, 100 })
#define DISASTER_THUNDER	({ "���p", THUNDER, WE_HUMIDITY_2 | WE_HUMIDITY_3 | WE_TEMPERATURE_2 | WE_TEMPERATURE_3 | WE_ALL_WIND, 100 })
#define DISASTER_TORNADO	({ "����", TORNADO, WE_HUMIDITY_2 | WE_HUMIDITY_3 | WE_ALL_TEMPERATURE | WE_WIND_3, 50 })
#define DISASTER_FIRE		({ "���a", FIRE, WE_HUMIDITY_0 | WE_ALL_TEMPERATURE | WE_ALL_WIND, 50 })
#define DISASTER_HAILSTONE	({ "�B�r", HAILSTONE, WE_HUMIDITY_3 | WE_TEMPERATURE_0 | WE_TEMPERATURE_1 | WE_ALL_WIND, 10 })
#define DISASTER_EARTHQUAKE	({ "�a�_", EARTHQUAKE, WE_ALL_HUMIDITY | WE_ALL_TEMPERATURE | WE_ALL_WIND, 2})
#define DISASTER_ERUPTION	({ "���s�z�o", ERUPTION, WE_ALL_HUMIDITY | WE_ALL_TEMPERATURE | WE_ALL_WIND, 1})
#define DISASTER_METEORITE	({ "�k��ŧ��", METEORITE, WE_ALL_HUMIDITY | WE_ALL_TEMPERATURE | WE_ALL_WIND, 1})

// �Ѯ𪬺A�w�q
#define WEATHER_CHINESE			0
#define WEATHER_ID			1
#define WEATHER_PROBABILITY		2
#define WEATHER_OCCUR_TERM		3
#define WEATHER_HOLDTIME_TERM		4
#define WEATHER_RELATED_DISASTER	5

// �U�ؤѮ�

#define WEATHER_SUNNY		({								\
	"����", SUNNY,	10,									\
	WE_ALL_SEASON | WE_HUMIDITY_0 | WE_HUMIDITY_1 | WE_ALL_WIND | WE_ALL_TEMPERATURE,	\
	([ 3:5, 4:5, 6:5, 12:3, 24:3 ]),							\
	({ DISASTER_ARIDITY }), 								\
				})

#define WEATHER_CLOUDY		({								\
	"����",	CLOUDY,	10,									\
	WE_ALL_SEASON | WE_HUMIDITY_1 | WE_HUMIDITY_2 | WE_ALL_WIND | WE_ALL_TEMPERATURE,	\
	([ 3:6, 4:6, 6:4, 12:3, ]),								\
	({ }),											\
				})

#define WEATHER_RAINY		({								\
	"�B��",	RAINY,	10,									\
	WE_ALL_SEASON | WE_HUMIDITY_1 | WE_HUMIDITY_2 | WE_ALL_WIND | WE_ALL_TEMPERATURE,	\
	([ 3:3, 4:3, 6:2, 12:1 ]),								\
	({ DISASTER_FLOOD }),									\
				})

#define WEATHER_WINDY		({								\
	"�j��",	WINDY, 10,									\
	WE_SUMMER | WE_HUMIDITY_0 | WE_WIND_2 | WE_WIND_3 | WE_TEMPERATURE_3,			\
	([ 3:3, 4:2, 6:1 ]),									\
	({ DISASTER_FIRE }),									\
				})
					
#define WEATHER_SNOWY		({								\
	"�U��",	SNOWY,	6,									\
	WE_SPRING | WE_WINTER | WE_HUMIDITY_2 | WE_HUMIDITY_3 | WE_ALL_WIND | WE_TEMPERATURE_0,		\
	([ 3:7, 4:6, 6:5, 12:3, 24:1 ]),							\
	({ DISASTER_COLD }),									\
				})

#define WEATHER_SHOWER		({								\
	"�}�B",	SHOWER,	6,									\
	WE_ALL_SEASON | WE_HUMIDITY_2 | WE_HUMIDITY_3 | WE_ALL_WIND | WE_TEMPERATURE_2 | WE_TEMPERATURE_3,	\
	([ 1:4, 2:3, 3:2, 4:1, ]),								\
	({ DISASTER_FLOOD, DISASTER_HAILSTONE, DISASTER_THUNDER }),				\
				})

#define WEATHER_TYPHOON		({								\
	"�䭷",	TYPHOON, 3,									\
	WE_SUMMER | WE_AUTUMN | WE_HUMIDITY_2 | WE_HUMIDITY_3 | WE_WIND_3 | WE_TEMPERATURE_2 | WE_TEMPERATURE_3,\
	([ 6:3, 12:4, 24:3 ]),									\
	({ DISASTER_FLOOD, DISASTER_THUNDER, DISASTER_TORNADO }),				\
				})

#define WEATHER_FOEHN		({								\
	"�I��",	FOEHN, 3,									\
	WE_SUMMER | WE_HUMIDITY_0 | WE_WIND_2 | WE_WIND_3 | WE_TEMPERATURE_3,			\
	([ 3:3, 4:2, 6:1 ]),									\
	({ DISASTER_FIRE }),									\
				})

				
#define ALL_WEATHER 	({ WEATHER_SUNNY, WEATHER_CLOUDY, WEATHER_RAINY, WEATHER_SNOWY, WEATHER_SHOWER, WEATHER_TYPHOON, WEATHER_FOEHN, WEATHER_WINDY })


// ��׾��v
#define HUMIDITY_PROBABILITY	\
([\
	SPRING	:	([ WE_HUMIDITY_0 : 1, WE_HUMIDITY_1 : 1, WE_HUMIDITY_2 : 1, WE_HUMIDITY_3 : 1, ]),\
	SUMMER	:	([ WE_HUMIDITY_0 : 4, WE_HUMIDITY_1 : 1, WE_HUMIDITY_2 : 1, WE_HUMIDITY_3 : 4, ]),\
	AUTUMN	:	([ WE_HUMIDITY_0 : 2, WE_HUMIDITY_1 : 3, WE_HUMIDITY_2 : 4, WE_HUMIDITY_3 : 1, ]),\
	WINTER	:	([ WE_HUMIDITY_0 : 3, WE_HUMIDITY_1 : 2, WE_HUMIDITY_2 : 2, WE_HUMIDITY_3 : 3, ]),\
])

// ���t���v
#define WIND_PROBABILITY	\
([\
	SPRING	:	([ WE_WIND_0 : 2, WE_WIND_1 : 4, WE_WIND_2 : 3, WE_WIND_3 : 1 ]),\
	SUMMER	:	([ WE_WIND_0 : 3, WE_WIND_1 : 2, WE_WIND_2 : 2, WE_WIND_3 : 3 ]),\
	AUTUMN	:	([ WE_WIND_0 : 3, WE_WIND_1 : 4, WE_WIND_2 : 2, WE_WIND_3 : 1 ]),\
	WINTER	:	([ WE_WIND_0 : 3, WE_WIND_1 : 3, WE_WIND_2 : 2, WE_WIND_3 : 1 ]),\
])
