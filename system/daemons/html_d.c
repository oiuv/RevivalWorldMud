/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : html_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-1-21
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <mudlib.h>


// �թ��� HEADER ----------------------------------------------------------------------
string WHITE_HEADER = @LONG
<html>
<!-- Created by ���ͪ��@�� %^UPDATETIME%^ -->
<head>
  <meta http-equiv=Content-Type content="text/html; charset=big5">
  %^REFRESH%^
  <title>%^TITLE%^</title>
<style><!--
 a{text-decoration:none}
        -->
  </style>
</head>
<body bgcolor="#ffffff" text="#55555" link="#666688" vlink="#666688">
<table width=80% align=center>
  <tr>
    <td align=center style='font-familiy:verdana;font-size:15px;color:#555;'>�������Ѩt�Φ۰ʫظm�A��s�ɶ��� %^UPDATETIME%^</td>
  </tr>
</table>
<hr width=80% align=center><p>
LONG;



// �©��� HEADER ----------------------------------------------------------------------
string BLACK_HEADER = @LONG
<html>
<!-- Created by ���ͪ��@�� %^UPDATETIME%^ -->
<head>
  <meta http-equiv=Content-Type content="text/html; charset=big5">
  %^REFRESH%^
  <title>%^TITLE%^</title>
<style><!--
 a{text-decoration:none}
        -->
  </style>
</head>
<body bgcolor="#000000" text="#bbbbbb" link="#666688" vlink="#666688">
<table width=80% align=center>
  <tr>
    <td align=center style='font-familiy:verdana;font-size:15px;color:#bbb;'>�������Ѩt�Φ۰ʫظm�A��s�ɶ��� %^UPDATETIME%^</td>
  </tr>
</table>
<hr width=80% align=center><p>
LONG;



// �թ��� FOOTER ----------------------------------------------------------------------
string WHITE_FOOTER = @LONG
<p><hr width=80% align=center>
<table width=80% align=center>
  <tr>
    <td align=center><font face=verdana size=2 color=555555>Copyright &copy;2000-2007 <a href="http://www.revivalworld.org"><font color=111111><u>���ͪ��@��</u></a> <font color=555555> All Rights Reserved</font></td>

  </tr>
</table>
</body>
</html>
<!-- Created by ���ͪ��@�� %^UPDATETIME%^ -->
LONG;


// �©��� FOOTER ----------------------------------------------------------------------
string BLACK_FOOTER = @LONG
<p><hr width=80% align=center>
<table width=80% align=center>
  <tr>
    <td align=center><font face=verdana size=2 color=bbbbbb>Copyright &copy;2000-2007 <a href="http://www.revivalworld.org"><font color=fffff><u>���ͪ��@��</u></a> <font color=bbbbbb> All Rights Reserved</font></td>

  </tr>
</table>
</body>
</html>
<!-- Created by ���ͪ��@�� %^UPDATETIME%^ -->
LONG;

mapping HI_TABLE_16 = 	
([
30		:	"<font color=999999>",
31		:	"<font color=ff0000>",
32		:	"<font color=00ff00>",
33		:	"<font color=ffff00>",
34		:	"<font color=0000ff>",
35		:	"<font color=ff00ff>",
36		:	"<font color=99ffff>",
37		:	"<font color=ffffff>",
]);

mapping LO_TABLE_16 =
([
30		:	"<font color=999999>",
31		:	"<font color=991111>",
32		:	"<font color=119911>",
33		:	"<font color=999911>",
34		:	"<font color=111199>",
35		:	"<font color=991199>",
36		:	"<font color=009999>",
37		:	"<font color=999999>",	
]);

mapping ANSI_TO_16;

string translate_to_html(string content)
{
	// ���N�r��
	content = replace_string(content, "  ", "�@");
	content = replace_string(content,"��","�i");
	content = replace_string(content, "\n", "<br>\n");

	foreach(string color, string b16 in ANSI_TO_16)
		content = replace_string(content, color, b16);
	
	content = remove_ansi(content);
	
	return content;
}

void make_html(string content, mapping index)
{
	string text;
	string bgcolor = index["bgcolor"] || "white";
	int fontsize = index["fontsize"] || 12;
	string fontface = index["fontface"] || "�s�ө���";
	string filename = index["filename"] || "/www/tmp.html";
	string title = index["title"] || "�L���D";
	int refresh = index["refresh"];
	

	text = bgcolor == "black" ? BLACK_HEADER : WHITE_HEADER;
	text = terminal_colour(text, ([
		"TITLE"		:	title,
		"REFRESH"	:	refresh?("<meta http-equiv='refresh' content='"+refresh+"'; url='"+filename+"'>"):"",
		"UPDATETIME"	:	TIME_D->replace_ctime(time()),
	]));
	
	text += "<div style='font-size:"+fontsize+"px;font-family:"+fontface+";'><nobr>";
	text += translate_to_html(content);
	text += "</div>";
	text += bgcolor == "black" ? BLACK_FOOTER : WHITE_FOOTER;
	

	write_file(filename, text, 1);
}


void create_map_html()
{
	int x, y, num;
	string map, text = "";
	array worldmapdata;
	int maptype;

	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px solid #555;width:600px;'><tr><td style='background:#fff;font-size:20px;color:#884;'><b>�D�Y�ɥ@�ɦa��</b><hr></td></tr></table>";
	
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;width:800px;'>";
	for(y=20;y>=-20;y--)
	{
		text += "<tr height=20>";
		
		for(x=-20;x<=20;x++)
		{
			worldmapdata = MAP_D->query_world_map(x, y);
			
			if( !worldmapdata )
				text += "<td width=20 bgcolor=#000000>�@</td>";
			else if( CITY_D->city_exist(worldmapdata[0], worldmapdata[1]) )
				text += "<td width=20 bgcolor=#555577><center><a href='map/citymap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank>"+(CITY_D->query_city_info(worldmapdata[0], "fallen") ? "<font size=3 color=555577>�i":"<font size=3 color=ffffff>"+remove_ansi(CITY_D->query_city_name(worldmapdata[0]))[0..1])+"</a></center></td>";
			else if( AREA_D->area_exist(worldmapdata[0], worldmapdata[1]) )
			{
				maptype = AREA_D->query_area_info(worldmapdata[0], "maptype");
				if( maptype == 0 )
					text += "<td width=20 bgcolor=#55aa55><center><a href='map/areamap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank><font size=3 color=55aa55>�i</a></td>";
				else if( maptype == 1 )
					text += "<td width=20 bgcolor=#000077><center><a href='map/areamap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank><font size=3 color=000077>�i</a></td>";
			}
		}
		
		text += "</tr>";
	}
	
	text += "</table>";
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;width:600px;'><tr height=30><td width=30 bgcolor=#555577>�@</td><td width=90>����</td><td width=30 bgcolor=#55aa55>�@</td><td width=90>����</td><td width=30 bgcolor=#000077>�@</td><td width=90>���v</td><td width=30 bgcolor=#000000>�@</td><td width=90>�L�ϰ�</td></tr></table><br><br><br>";

	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px solid #555;width:600px;'><tr><td style='background:#fff;font-size:20px;color:#448;'><b>�D�Y�ɫ����a��</b><hr></td></tr></table>";
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;padding:3px;width:600px;'>";
	foreach(map in sort_array(CITY_D->query_cities(), (: sizeof(CITY_D->query_city_info($1, "citizens")) < sizeof(CITY_D->query_city_info($2, "citizens")) ? 1 : -1 :)))
	{
		if( map == "wizard" ) continue;

		num = 0;
		while(CITY_D->city_exist(map, num))
		{
			text += "<tr>";
			text += "<td style='font-size:18px;width:100px;background:#000;'>"+CITY_D->query_city_name(map)+"</b></td>";
			text += "<td style='font-size:18px;width:200px;background:#000;'>"+(CITY_D->query_section_info(map, num, "name")||"�� "+(num+1)+" �ìP����")+"</td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='/online/rw/map/citymap_"+replace_string(map, " ", "%20")+"_"+num+".html' target=_blank><font color=#ffffff>HTML��r��</font></a></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=citymap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=10' target=_blank><font color=#ffffff>JAVA��(�p)</font></a></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=citymap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=14' target=_blank><font color=#ffffff>JAVA��(�j)</font></a></td>";
			text += "</tr>";
			num++;
		}
		text += "<tr><td colspan=5 style='border-top:1px solid #111;background:#000;font-size:1px;'>&nbsp;</td></tr>";
	}
	text += "</table><br><br><br>";
	
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px solid #555;width:600px;'><tr><td style='background:#fff;font-size:20px;color:#484;'><b>�D�Y�ɭ��Ϧa��</b><hr></td></tr></table>";
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;padding:3px;width:600px;'>";
	foreach(map in AREA_D->query_areas())
	{
		num = 0;
		while(AREA_D->area_exist(map, num))
		{
			text += "<tr>";
			text += "<td style='font-size:18px;width:100px;background:#000;'>"+AREA_D->query_area_name(map)+"</td>";
			text += "<td style='font-size:18px;width:200px;background:#000;'>"+(AREA_D->query_section_info(map, num, "name")||"�� "+(num+1)+" ����")+"</td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='/online/rw/map/areamap_"+replace_string(map, " ", "%20")+"_"+num+".html' target=_blank><font color=#ffffff>HTML��r��</a></font></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=areamap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=10' target=_blank><font color=#ffffff>JAVA��(�p)</font></a></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=areamap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=14' target=_blank><font color=#ffffff>JAVA��(�j)</font></a></td>";
			text += "</tr>";
			num++;
		}
		text += "<tr><td colspan=5 style='border-top:1px solid #111;background:#000;font-size:1px;'>&nbsp;</td></tr>";
	}
	text += "</table>";
	
	make_html(text, (["filename":"/www/map.html", "title":"�Y�ɦa��", "fontsize":15,"fontface":"�ө���"]));
}

void create_stock_html()
{
	int number;
	string enterprise;
	mapping data;
	string text = "<center><table cellpadding=0 cellspacing=0 style='border:3px double #555;padding:10px;'>";
	
	foreach(enterprise, data in ENTERPRISE_D->query_enterprises())
	{
		number = data["enterprise_id_number"];
		text += "<tr><td style='font-size:15px;color:#fff;background:#000;'>���~�W��:"+data["color_id"]+"</td>";
		text += "<td style='font-size:15px;color:#000'>�ثe�ѻ�:"+data["stockvalue"]+"</td></tr>";
		text += "<tr><td colspan=2><a href='/online/rw/mrtg/stock/"+number+"/stock_"+number+".html'><img src='/online/rw/mrtg/stock/"+number+"/stock_"+number+"-day.png'></a></td></tr>";
	}

	text += "</table>";
	make_html(text, (["filename":"/www/stock.html", "title":"�Y�ɪѲ�", "fontsize":15, "fontface":"�ө���"]));
}

void create_html()
{
	create_map_html();
	create_stock_html();
}
	
void create()
{
	int fcolor, bcolor;
	
	ANSI_TO_16 = allocate_mapping(0);

	ANSI_TO_16[ESC+"[0m"] = "<font color=999999>";
	ANSI_TO_16[ESC+"[m"] = "<font color=999999>";

	for(fcolor=30;fcolor<=37;fcolor++)
	{
		ANSI_TO_16[ESC+"["+fcolor+"m"] = LO_TABLE_16[fcolor];
		ANSI_TO_16[ESC+"[0;"+fcolor+"m"] = LO_TABLE_16[fcolor];
		ANSI_TO_16[ESC+"[1;"+fcolor+"m"] = HI_TABLE_16[fcolor];
		ANSI_TO_16[ESC+"["+fcolor+";1m"] = LO_TABLE_16[fcolor];
	
		for(bcolor=41;bcolor<=47;bcolor++)
		{
			ANSI_TO_16[ESC+"["+fcolor+";"+bcolor+"m"] = HI_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"["+bcolor+";"+fcolor+"m"] = HI_TABLE_16[fcolor];

			ANSI_TO_16[ESC+"[0;"+fcolor+";"+bcolor+"m"] = LO_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"[0;"+bcolor+";"+fcolor+"m"] = LO_TABLE_16[fcolor];
			
			ANSI_TO_16[ESC+"[1;"+fcolor+";"+bcolor+"m"] = HI_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"[1;"+bcolor+";"+fcolor+"m"] = HI_TABLE_16[fcolor];
			
			ANSI_TO_16[ESC+"["+fcolor+";1;"+bcolor+"m"] = HI_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"["+bcolor+";1;"+fcolor+"m"] = HI_TABLE_16[fcolor];
			
			ANSI_TO_16[ESC+"["+fcolor+";"+bcolor+";1m"] = HI_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"["+bcolor+";"+fcolor+";1m"] = HI_TABLE_16[fcolor];
		}
	}
	
	create_html();
}

string query_name()
{
	return "HTML �t��(HTML_D)";
}
