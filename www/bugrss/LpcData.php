<?php
class LpcData
{
    var $file;
    var $rFile;
    var $data;
    var $ptr;
    function init($sencode)
    {
	if($sencode){
		$strs = file($this->file);
		foreach($strs as $str) {
			$str = eregi_replace("\033[[0-9]*;*[0-9]*;*[0-9]*;*[0-9]*m","",$str);
			$this->rFile[] = iconv($sencode,"utf-8", $str);
		}
	}
	else
		$this->rFile = file($this->file);
    }
    function processLpcData()
    {
        foreach($this->rFile as $l)
        {
            if($l[0] == '#' ) continue;

            $t = explode(" ",$l);
            $this->ptr = 2;
            $tmp = implode(" ",array_slice($t,1));
            if(substr($tmp,0,2) == "([")
                $this->data[$t[0]] = $this->processMapping($tmp);
            else if(substr($tmp,0,2) == "({")
                $this->data[$t[0]] = $this->processArray($tmp);
            else if(substr($l,0,2) == "(["){
		$this->data["data"] = $this->processMapping($l);
	    }
	    else if(substr($l,0,2) == "({"){
                $this->data["data"] = $this->processArray($l);
	    }
   	    else
		$this->data[0] = $l;
        }
    }

    function processMapping($tmp)
    {
        $i = $this->ptr;
        $t = $i;
	
        while($i++)
        {
            if($tmp[$i] == '"') break;
        }
        $ts = substr($tmp,$t+1,$i - $t-1); 
        $i++;
        for(;$i<strlen($tmp);$i++)
        {
	    if($tmp[$i] == ')')
            {
                $i++;
		$this->ptr = $i;
                break;
            }
            switch($tmp[$i])
            {
                case ':':
                break;
                case ',': // ,�}�Y�A�N�N��n�B�z�U�@�ӤF�A�]���O map �ҥH�}�Y�@�w�O"
                    $i++;
                    if($tmp[$i] == ']')
                    {
			$this->ptr = $i;
                        break;
                    }                   
                    $this->ptr = $i;
                    $ts = $this->processString($tmp);
                    $i = $this->ptr;

                break;
                case '"': // �r�ꪺ�}�l
                    $this->ptr = $i;
                    $map[$ts] = $this->processString($tmp);
                    $i = $this->ptr;
                break;
                case '(':
                    $i++;
                    switch($tmp[$i])
                    {
                        case '[': // �S�n�^�Y�B�z mapping �F

                            $i++;
                            $this->ptr = $i;
                            $map[$ts] = $this->processMapping($tmp);
                            $i = $this->ptr - 1;
                        break;
                        case '{': // �B�z array
                            
                            $i++;
                            $this->ptr = $i;
                            $map[$ts] = $this->processArray($tmp);
                            $i = $this->ptr;
                             
                        break;
                        default:
                    }
                break;  
                case '-': // �H�U���Ʀr���}�l
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    $this->ptr = $i;
                    $map[$ts] = $this->processNumber($tmp);
                    $i = $this->ptr;
                    $i--;
                    
                break;
                default:
            }
        }
        $this->ptr = $i;
        //print_r($map);
        return $map;
    }
    function processArray($tmp)
    {
        $i = $this->ptr;
        $t = $i;

        for(;$i<strlen($tmp);$i++)
        {
            if($tmp[$i] == '}' )
            {
                $i+=1;
		$this->ptr = $i;
                break;
            }
            switch($tmp[$i])
            {
    
                case ',': // ,�}�Y�A�N�N��n�B�z�U�@�ӤF�A�]���O map �ҥH�}�Y�@�w�O"
                break;
                case '"': // �r�ꪺ�}�l
                    $this->ptr = $i;
                    $arr[] = $this->processString($tmp);
                    $i = $this->ptr;
                break;
                case '(':
                    $i++;
                    switch($tmp[$i])
                    {
                        case '[': // �S�n�^�Y�B�z mapping �F
                            $i++;
                            $this->ptr = $i;
                            $arr[]= $this->processMapping($tmp);
                            $i = $this->ptr - 1;
                        break;
                        case '{' :// �B�z array
                            $i++;
                            $this->ptr = $i;
                            $arr[]= $this->processArray($tmp);
                            $i = $this->ptr;
                        break;
                        default:
                    }
                break;  
                case '-': // �H�U���Ʀr���}�l
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    $this->ptr = $i;
                    $arr[] = $this->processNumber($tmp);
                    $i = $this->ptr;
                break;
                default:
            }
        }
        return $arr;    
    }
    function processNumber($tmp)
    {
        $i = $this->ptr;
        $t = $i;
        $i--;
        while($i++)
        {
            // ��,�N�O�o�� �Ʀr ������
            if($tmp[$i] == ',') break;
        }
        $ts = substr($tmp,$t,$i - $t) ; 

        // �[�W�@�ӡA�����L�ܦ��٬O,�}�Y
        $this->ptr = $i;
        return $ts;  
    }
    function processString($tmp)
    {
        $i = $this->ptr;
        $t = $i;
        
        while($i++)
        {
            // "���ܡA�N��o�Ӧr�굲���F

            if($tmp[$i] == '"') break;
            // ���]�o�ӬO \ ���ܡA�U�@�ӴN�Q������
            if($tmp[$i] == '\\') $i++;
            
        }
        $ts = substr($tmp,$t+1,$i - $t -1 );
        $this->ptr = $i;
        return $ts;
    }
}
?>
