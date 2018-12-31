// question_d.c
#include <ansi.h>
#include <daemon.h>
#include <message.h>
#define Q_TIME  20 * 60

nosave mapping questions = allocate_mapping(0);
nosave string question, answer;
nosave int last_time, interest;
nosave object *answer_obs;

int load_data(string file);
void question_request(string q, string ans);
string random_question();
void remove_answer_ob();
void regular_question();
void pascal_question();
void assemble_question();

void create()
{

	string file = "/system/daemons/etc/questions";

	if( !load_data(file) )
	{
		printf("Error: �L�k���J�ɮ� " + file + "�C\n");
		return;
	}
	//set_heart_beat(10);
	return;
}
int load_data(string file)
{
	string question, answer, lines, line;

	if( file_size(file) < 0 ) return 0;
	lines = read_file(file);
	foreach(line in explode(lines, "\n"))
	{
		if( sscanf(line, "%s<>%s", question, answer) == 2 )
		{
			questions[question] = answer;
		}
	}
	return 1;
}
void heart_beat()
{
	return;
	if( (time() - last_time) < Q_TIME ) return;
	last_time = time();
	remove_answer_ob();
	switch(random(5))
	{
	case 0..1:	assemble_question();	break;
	case 2..3:	pascal_question();	break;	
	default:	regular_question();	break;
	}
}
string random_question()
{
	string *q;
	q = keys(questions);
	return q[random(sizeof(q))];
}
void question_request(string q, string ans)
{
	object *ob;
	ob = filter_array(users(), (: environment($1) && $1->is_user_ob() :));
	ob = filter_array(users(), (: random(10) < 4 :)); // �H���D�缾�a
	if( !sizeof(ob) ) return;
	tell(ob, HIY"���D�G" + q + " (�Х� answer �^��)\n"NOR);
	question = q;
	answer = ans;
	answer_obs = ob;
	interest = 1000;
}
int valid_answer(object ob)
{
	if( member_array(ob, answer_obs) < 0 ) return 0;
	return 1;
}
int answer_incoming(object ob, string arg)
{
	if( !answer_obs ) return 0;
	if( !valid_answer(ob) ) return 0;

	answer_obs -= ({ ob });

	if( !arg ) return 0;

	if( arg != answer )
	{
		return -1;
	}
	tell(ob, HIY"����F�A�A�o�� $CT " + interest + "�C\n"NOR, CMDMSG);
	ob->earn_money("CT", interest);
	interest = to_int(to_float(interest)/1.3);
	return 1;
}
void remove_answer_ob()
{
	if( !answer_obs || !sizeof(answer_obs) ) return;
	foreach(object ob in answer_obs)
	{
		if( !ob ) continue;
		//ob->directly_receive(HIR"�A�S���^�����D�A�@�A $CT 100�C\n"NOR);
		//set("money/CT", count(query("money/CT", ob), "-", 100), ob);
		answer_obs -= ({ ob });
	}
}
mapping query_questions()
{
	return questions;
}
string query_question()
{
	return question;
}
string query_answer()
{
	return answer;
}

/* Regular Question */
void regular_question()
{
	string q = random_question();
	question_request(q, questions[q]);
}

/* Pascal Math Question */
mapping pascal(int i)
{
	int j, k;
	mapping arr, *arrs;

	arrs = allocate(0);
	for(j=0; j<=i+1; j++)
	{
		arr = allocate_mapping(0);
		for(k=0; k<j; k++)
		{
			if( k == 0 || k == j-1 )
			{
				arr[k] = 1;
				continue;
			}
			arr[k] = arrs[j-1][k-1] + arrs[j-1][k];
		}
		arrs += ({ arr });
	}
	return arrs[i+1];
}

void pascal_question()
{
	mapping arr;
	int i, j, ans, x, y, a, b;

	i = random(10)+1;
	arr = pascal(i);
	j = random(sizeof(arr)-1);
	ans = arr[j];
	x = i - j;
	y = i - x;
	a = random(9)+2;
	b = random(9)+2;
	ans = ans * pow(a, x) * pow(b, y);
	question_request("("+a+"x+"+b+"y)^"+i+" �i�}�� x^"+x+"*y^"+y+" ���Y�ƬO�h�֡H", sprintf("%d", ans));
}

void assemble_question()
{
	int i, ch, t, x, y;
	string *a, *b, ans, q;

	a = allocate(0);
	b = allocate(0);
	x = random(4) + 1;
	y = random(4) + 1;
	ch = 'a';
	for(i=0; i<x; i++)
	{
		ch += random(3) + 1;
		a += ({ sprintf("%c", ch) });
	}
	ch = 'a';
	for(i=0; i<y; i++)
	{
		ch += random(3) + 1;
		b += ({ sprintf("%c", ch) });
	}
	t = random(2);
	q = sprintf("�ըD���X {%s} %s {%s}�H (�Ы��r�����ǱƦC)",
	    implode(a, ","),
	    (t ? "�涰": "�p��"),
	    implode(b, ","));
	if( t ) ans = implode(sort_array(a & b, 1), ",");
	else ans = implode(sort_array(a | b, 1), ",");
	question_request(q, "{" + ans + "}");
}
string query_name()
{
	return "�D�ݨt��(QUESTION_D)";
}
