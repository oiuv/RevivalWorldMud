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


// 白底色 HEADER ----------------------------------------------------------------------
string WHITE_HEADER = @LONG
<html>
<!-- Created by 重生的世界 %^UPDATETIME%^ -->
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
    <td align=center style='font-familiy:verdana;font-size:15px;color:#555;'>本頁面由系統自動建置，更新時間為 %^UPDATETIME%^</td>
  </tr>
</table>
<hr width=80% align=center>
LONG;



// 黑底色 HEADER ----------------------------------------------------------------------
string BLACK_HEADER = @LONG
<html>
<!-- Created by 重生的世界 %^UPDATETIME%^ -->
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
    <td align=center style='font-familiy:verdana;font-size:15px;color:#bbb;'>本頁面由系統自動建置，更新時間為 %^UPDATETIME%^</td>
  </tr>
</table>
<hr width=80% align=center>
LONG;



// 白底色 FOOTER ----------------------------------------------------------------------
string WHITE_FOOTER = @LONG
<p><hr width=80% align=center>
<table width=80% align=center>
  <tr>
    <td align=center><font face=verdana size=2 color=555555>Copyright &copy;2000-2007 </font><a href="http://www.revivalworld.org"><font color=111111><u>重生的世界</u></font></a> <font color=555555> All Rights Reserved</font></td>

  </tr>
</table>
</body>
</html>
<!-- Created by 重生的世界 %^UPDATETIME%^ -->
LONG;


// 黑底色 FOOTER ----------------------------------------------------------------------
string BLACK_FOOTER = @LONG
<p><hr width=80% align=center>
<table width=80% align=center>
  <tr>
    <td align=center><font face=verdana size=2 color=bbbbbb>Copyright &copy;2018-2019</font> <a href="https://github.com/oiuv/RevivalWorldMud"><font color=fffff><u>重生的世界</u></font></a> <font color=bbbbbb> All Rights Reserved</font></td>

  </tr>
</table>
</body>
</html>
<!-- Created by 重生的世界 %^UPDATETIME%^ -->
LONG;

mapping HI_TABLE_16 =
([
30		:	"</font><font color=999999>",
31		:	"</font><font color=ff0000>",
32		:	"</font><font color=00ff00>",
33		:	"</font><font color=ffff00>",
34		:	"</font><font color=0000ff>",
35		:	"</font><font color=ff00ff>",
36		:	"</font><font color=99ffff>",
37		:	"</font><font color=ffffff>",
]);

mapping LO_TABLE_16 =
([
30		:	"</font><font color=999999>",
31		:	"</font><font color=991111>",
32		:	"</font><font color=119911>",
33		:	"</font><font color=999911>",
34		:	"</font><font color=111199>",
35		:	"</font><font color=991199>",
36		:	"</font><font color=009999>",
37		:	"</font><font color=999999>",
]);

mapping ANSI_TO_16;

string translate_to_html(string content)
{
	// 替代字元
	content = replace_string(content, "  ", "　");
	content = replace_string(content,"□","█");
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
	string fontface = index["fontface"] || "新細明體";
	string filename = index["filename"] || "/www/tmp.html";
	string title = "重生的世界(Map)--Mud.Ren" || index["title"];
	int refresh = index["refresh"];


	text = bgcolor == "black" ? BLACK_HEADER : WHITE_HEADER;
	text = terminal_colour(text, ([
		"TITLE"		:	title,
		"REFRESH"	:	refresh?("<meta http-equiv='refresh' content='"+refresh+"; url="+filename+"'>"):"",
		"UPDATETIME"	:	TIME_D->replace_ctime(time()),
	]));

	text += "<div style='font-size:"+fontsize+"px;font-family:"+fontface+";text-align:center;'><font>";
	text += translate_to_html(content);
	text += "</font></div>";
	text += bgcolor == "black" ? BLACK_FOOTER : WHITE_FOOTER;


	write_file(filename, text, 1);
}


void create_map_html()
{
	int x, y, num;
	string map, text = "";
	array worldmapdata;
	int maptype;

	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px solid #555;width:600px;'><tr><td style='background:#fff;font-size:20px;color:#884;'><b>．即時世界地圖</b><hr></td></tr></table>";

	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;width:800px;'>";
	for(y=20;y>=-20;y--)
	{
		text += "<tr height=20>";

		for(x=-20;x<=20;x++)
		{
			worldmapdata = MAP_D->query_world_map(x, y);

			if( !worldmapdata )
				text += "<td width=20 bgcolor=#000000>　</td>";
			else if( CITY_D->city_exist(worldmapdata[0], worldmapdata[1]) )
				text += "<td width=20 bgcolor=#555577><center><a href='map/citymap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank>"+(CITY_D->query_city_info(worldmapdata[0], "fallen") ? "<font size=3 color=555577>█":"<font size=3 color=ffffff>"+remove_ansi(CITY_D->query_city_name(worldmapdata[0]))[0..1])+"</a></center></td>";
			else if( AREA_D->area_exist(worldmapdata[0], worldmapdata[1]) )
			{
				maptype = AREA_D->query_area_info(worldmapdata[0], "maptype");
				if( maptype == 0 )
					text += "<td width=20 bgcolor=#55aa55><center><a href='map/areamap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank><font size=3 color=55aa55>█</a></td>";
				else if( maptype == 1 )
					text += "<td width=20 bgcolor=#000077><center><a href='map/areamap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank><font size=3 color=000077>█</a></td>";
			}
		}

		text += "</tr>";
	}

	text += "</table>";
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;width:600px;'><tr height=30><td width=30 bgcolor=#555577>　</td><td width=90>城市</td><td width=30 bgcolor=#55aa55>　</td><td width=90>郊區</td><td width=30 bgcolor=#000077>　</td><td width=90>海洋</td><td width=30 bgcolor=#000000>　</td><td width=90>無區域</td></tr></table><br><br><br>";

	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px solid #555;width:600px;'><tr><td style='background:#fff;font-size:20px;color:#448;'><b>．即時城市地圖</b><hr></td></tr></table>";
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;padding:3px;width:600px;'>";
	foreach(map in sort_array(CITY_D->query_cities(), (: sizeof(CITY_D->query_city_info($1, "citizens")) < sizeof(CITY_D->query_city_info($2, "citizens")) ? 1 : -1 :)))
	{
		if( map == "wizard" ) continue;

		num = 0;
		while(CITY_D->city_exist(map, num))
		{
			text += "<tr>";
			text += "<td style='font-size:18px;width:100px;background:#000;'>"+CITY_D->query_city_name(map)+"</b></td>";
			text += "<td style='font-size:18px;width:200px;background:#000;'>"+(CITY_D->query_section_info(map, num, "name")||"第 "+(num+1)+" 衛星都市")+"</td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='/www/map/citymap_"+replace_string(map, " ", "%20")+"_"+num+".html' target=_blank><font color=#ffffff>HTML文字圖</font></a></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=citymap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=10' target=_blank><font color=#ffffff>JAVA圖(小)</font></a></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=citymap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=14' target=_blank><font color=#ffffff>JAVA圖(大)</font></a></td>";
			text += "</tr>";
			num++;
		}
		text += "<tr><td colspan=5 style='border-top:1px solid #111;background:#000;font-size:1px;'>&nbsp;</td></tr>";
	}
	text += "</table><br><br><br>";

	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px solid #555;width:600px;'><tr><td style='background:#fff;font-size:20px;color:#484;'><b>．即時郊區地圖</b><hr></td></tr></table>";
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;padding:3px;width:600px;'>";
	foreach(map in AREA_D->query_areas())
	{
		num = 0;
		while(AREA_D->area_exist(map, num))
		{
			text += "<tr>";
			text += "<td style='font-size:18px;width:100px;background:#000;'>"+AREA_D->query_area_name(map)+"</td>";
			text += "<td style='font-size:18px;width:200px;background:#000;'>"+(AREA_D->query_section_info(map, num, "name")||"第 "+(num+1)+" 分區")+"</td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='/www/map/areamap_"+replace_string(map, " ", "%20")+"_"+num+".html' target=_blank><font color=#ffffff>HTML文字圖</a></font></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=areamap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=10' target=_blank><font color=#ffffff>JAVA圖(小)</font></a></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=areamap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=14' target=_blank><font color=#ffffff>JAVA圖(大)</font></a></td>";
			text += "</tr>";
			num++;
		}
		text += "<tr><td colspan=5 style='border-top:1px solid #111;background:#000;font-size:1px;'>&nbsp;</td></tr>";
	}
	text += "</table>";

	make_html(text, (["filename":"/www/map.html", "title":"即時地圖", "fontsize":15,"fontface":"細明體"]));
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
		text += "<tr><td style='font-size:15px;color:#fff;background:#000;'>企業名稱:"+data["color_id"]+"</td>";
		text += "<td style='font-size:15px;color:#000'>目前股價:"+data["stockvalue"]+"</td></tr>";
		text += "<tr><td colspan=2><a href='/www/mrtg/stock/"+number+"/stock_"+number+".html'><img src='/www/mrtg/stock/"+number+"/stock_"+number+"-day.png'></a></td></tr>";
	}

	text += "</table>";
	make_html(text, (["filename":"/www/stock.html", "title":"即時股票", "fontsize":15, "fontface":"細明體"]));
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

	ANSI_TO_16[ESC+"[0m"] = "</font><font color=999999>";
	ANSI_TO_16[ESC+"[m"] = "</font><font color=999999>";

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
	return "HTML 系統(HTML_D)";
}
