#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <sys/stat.h>
#include <stdbool.h>

 char* strdup(const char* str)
 {
	   char* newstr = (char*) malloc( strlen( str) + 1);
 
	   if (newstr) {
		   strcpy( newstr, str);
	   }
 
	   return newstr;
 }

 xmlDocPtr	  doc;
 xmlNodePtr   cur;
 xmlChar	  *uri;


 void read_xml(char *file,xmlChar *node, char *attr)
{


  //xmlDocPtr    doc;
  //xmlNodePtr   cur;
  //xmlChar      *uri;

  doc = xmlParseFile(file);

  cur = xmlDocGetRootElement(doc);

  cur = cur->xmlChildrenNode;

  while (cur != NULL) {

      if ((!xmlStrcmp(cur->name,(const xmlChar *) node))) {
        uri = xmlGetProp(cur, attr);
        //printf("uri: %s\n", uri);
        //xmlFree(uri);
	break;
      }
      cur = cur->next;
  }
  //xmlFreeDoc(doc);

}


char *read_attribute(char *file,xmlChar *node, char *attr)
{
  read_xml(file,node, attr);
  char * dnm=strdup(uri);
  xmlFree(uri);
  xmlFreeDoc(doc);
  return dnm;
}

bool create_directory(char *path)
{
  mkdir(path,0777);
  system("sync;sync");
  return true;
}

/*******************************************************/
/**** system komut gonderimi iicn kullanilir ******************/
/**** system_command : komut dizisi ***********************/
/**** command_return : komut icra sonrasi donen deger *******/
/**** ornek : dizinde ki dosya sayisi    ls -l | grep ^- | wc -l  *****/
/**** ornek : dizinde ki dizin sayisi    ls -l | grep ^d | wc -l *****/
/**** ornek : dizinde ki x adet dosya listesinden y nolu dosyanin ismi    ls | sort -n | head -x | tail -y *****/

/*******************************************************/

void sytem_command(char *system_command,char *command_return)
{
 char Temp_Adr[150]={0};
 char *dnm;
  //sprintf(Temp_Adr,"ls -l | grep ^- | wc -l > %s;sync;sync",path);
  sprintf(Temp_Adr,"%s;sync;sync",system_command);
  //system(Temp_Adr);

  FILE *fp;
  char _path[1035];

  /* Open the command for reading. */
  //fp = popen("ls -l | grep ^- | wc -l", "r");
  fp = popen(Temp_Adr, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  //printf(".....sytem_command\n", /*_path*/command_return);
  /* Read the output a line at a time - output it. */
  while (fgets(command_return, 256/*sizeof(command_return)-1*/, fp) != NULL) {
    //printf("sytem_command=%s\n", /*_path*/command_return);
  }

  //remove newline character from pointer
  int len;
  len = strlen(command_return);
if( command_return[len-1] == '\n' )
    command_return[len-1] = 0;
  
  /* close */
  pclose(fp);

}

void count_file(char *path)
{
  char Temp_Adr[150]={0};
  //sprintf(Temp_Adr,"ls -l | grep ^- | wc -l > %s;sync;sync",path);
  sprintf(Temp_Adr,"ls -l | grep ^- | wc -l;sync;sync");
  //system(Temp_Adr);

  FILE *fp;
  char _path[1035];

  /* Open the command for reading. */
  //fp = popen("ls -l | grep ^- | wc -l", "r");
  fp = popen(Temp_Adr, "r");
  if (fp == NULL) {
    //printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(_path, sizeof(_path)-1, fp) != NULL) {
    //printf("%s", _path);
  }

  /* close */
  pclose(fp);
}

void count_directory(char *path)
{
  char Temp_Adr[150]={0};
  sprintf(Temp_Adr,"ls -l | grep ^d | wc -l > %s;sync;sync",path);
  system(Temp_Adr);
}

Read_file(char *file_name_,char *buffer_)
{
  FILE *file;
  char *_buffer;
  unsigned long filelength;
  int length_loop;

  //open file
  file=fopen(file_name_,"r");
  if(!file)
  	{
  	  fprintf(stderr,"Unable to open file %s",file_name_);
	  return false;
  	}

  //Get file length
  fseek(file,0,SEEK_END);
  filelength=ftell(file);
  fseek(file,0,SEEK_SET);

  //Allocate memory
  _buffer=(char *)malloc(filelength+1);
  if(!_buffer)
  	{
  	  fprintf(stderr,"Memory error");
	  fclose(file);
	  return false;
  	}

  //read file contentsinto buffer
  fread(_buffer,filelength,1,file);
  fclose(file);
}


bool get_filename(char *path)
{
  char Temp_Adr[150]={0};
  char * buffer;

   FILE *file;
  unsigned long filelength;
  int length_loop;

  //open file
  file=fopen(path,"r");
  if(!file)
  	{
  	  fprintf(stderr,"Unable to open file %s",path);
	  return false;
  	}

  //Get file length
  fseek(file,0,SEEK_END);
  filelength=ftell(file);
  fseek(file,0,SEEK_SET);

  //Allocate memory
  buffer=(char *)malloc(filelength+1);
  if(!buffer)
  	{
  	  fprintf(stderr,"Memory error");
	  fclose(file);
	  return false;
  	}

  //read file contentsinto buffer
  fread(buffer,filelength,1,file);
  
  //Read_file(path,buffer);
  //printf("%s\n",buffer);
  fclose(file);
  //sprintf(Temp_Adr,"ls | sort -n | head -3 | tail -1 > %s;sync;sync",path);
  //system(Temp_Adr);
}


/*****************************************/
/*** fileordirectory : 1:file, 0:directory *********/
/*** create : 1:bulunamayan dosyayi olustur, 0:bulunamayan dosyayi olusturma *********/
/****************************************/
bool check_usb_dir(char *path,bool fileordirectory,bool create)
{
  char Temp_Adr[150]={0};
  struct stat statbuf;
  //printf("check_usb_dir \n");
  if(stat(path,&statbuf)!=-1)
  	{
  		if(S_ISDIR(statbuf.st_mode))
  			{
  			    //printf("hepsi var \n");
  				return true;
  			}
		
  	}
  else
  	{
  		if(!fileordirectory)
  			{
				mkdir(path,0777);
				//printf("directory yok \n");
  			}
			else
				{
				   if(create)
				   	{
				    	//printf("file yok \n");
						sprintf(Temp_Adr,"touch %s;sync;sync",path);
						system(Temp_Adr);
				   	}
				}
			return false;
  	}
  //printf("hepsi var \n");
  return true;
}

// /home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio
void search_copy_replace_file(char *folder_path)
{

		  int xml_file_loop=0;
		  int file_loop=0;
		  int filecount_infolder=0;
		  char tempcommand[150]={0};
		  char tempcommand2[150]={0};
		  bool dosyayi_indir=false;
		  bool same_dif_file_detect[25]={0};
		  char command_return[100]={0};
		  char *main_file_count;
		  char *main_sha;
		  char *main_filename;
		  
	  	  printf("DS/DS_TEMP/Main/MainAudio dizin var.Dosya copy islemi baslat..\n");
		  sprintf(tempcommand,"ls -l %s | grep ^- | wc -l",folder_path);
		  sytem_command(tempcommand,command_return);	
		  //sytem_command("ls -l /home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio | grep ^- | wc -l",command_return);
			filecount_infolder=atoi(command_return);
		  printf("dosya sayisi:%d.... \n",filecount_infolder);
		  main_file_count=read_attribute("/home/metin/Desktop/dnm1/xml/ds2.xml","MainAudio", "filecount");
  		  printf("main_file_count=%s \n ..................\n",main_file_count);
		  for(xml_file_loop=1;xml_file_loop<=atoi(main_file_count);xml_file_loop++)
		  	{
		  	    sprintf(tempcommand,"%s%d","sha",xml_file_loop);
		  		main_sha=read_attribute("/home/metin/Desktop/dnm1/xml/ds2.xml","MainAudio",tempcommand );
  		  		printf("xml_file_loop %d =%s \n ............\n",xml_file_loop,main_sha);
				file_loop=1;
				for (file_loop=1;file_loop<=filecount_infolder;file_loop++)
					{
					   sprintf(tempcommand2,"ls %s | sort -n | head -%d | tail -1",folder_path,file_loop);
						printf("....file_loop=%d %s\n",file_loop,tempcommand2);
						memset(&command_return[0], 0, sizeof(command_return));
					   sytem_command(tempcommand2,command_return);
					   //printf("....%s\n",command_return);
					   sprintf(tempcommand2,"sha1sum %s%s | cut -d ' ' -f 1",folder_path,command_return);
						//printf("....%s\n",tempcommand2);
					  sytem_command(tempcommand2,command_return);
					  //printf("....%s\n",command_return);
					  if(strcmp(command_return, main_sha) == 0)
					  	{
					  	  
						  printf("SHA COMPARING.....%s == %s .......BU DOSYANIN AYNISI VAR ISIM DEGISMELI....\n",command_return,main_sha);
						  dosyayi_indir=false;
						  same_dif_file_detect[file_loop-1]|=1;//benzer dosyalar bulunuyor
						  break;
					  	}
					  else
					  	{
					  	  printf("SHA COMPARING.....%s != %s .......BU DOSYA INDIRILMELI....\n",command_return,main_sha);
						  dosyayi_indir=true;
					  	}
					}

				if(dosyayi_indir)
					{
					    sprintf(tempcommand,"%s%d","file",xml_file_loop);
		  				main_filename=read_attribute("/home/metin/Desktop/dnm1/xml/ds2.xml","MainAudio",tempcommand );
					    printf(".......BU DOSYA (%s) INDIRILMELI....\n",main_filename);
						
					}
		  	}
		  for(file_loop=0;file_loop<filecount_infolder;file_loop++)
					{
					 if(same_dif_file_detect[file_loop])
					 	{
					 	   printf("....%d. dosya ayni kalsin\n",file_loop);
					 	}
					 else
					 	{
					 		printf("....%d. dosya farkli. Silinmeli.\n",file_loop);
					 	}
					}

		  printf(".....ISLEM TAMAMLANDI TEMP DIZINDEN ANA DIZINE KOPYALAMA ISLEMI GERCEKLESTIR \n ..................\n",main_file_count);
}


int main(int argc, char **argv) {

  #if 0
  char         *docname;
  xmlDocPtr    doc;
  xmlNodePtr   cur;
  xmlChar      *uri;

  if (argc <= 1) {
    printf("Usage: %s docname\n", argv[0]);
    return(0);
  }

  docname = argv[1];

  doc = xmlParseFile(docname);
printf("xmlParseFile \n ");
  cur = xmlDocGetRootElement(doc);
printf("xmlDocGetRootElement \n ");
  cur = cur->xmlChildrenNode;
printf("cur->xmlChildrenNode \n ");
  while (cur != NULL) {
printf("cur != NULL \n ");
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TV_Info"))) {
        uri = xmlGetProp(cur, "ProjectCount");
        printf("uri: %s\n", uri);
        xmlFree(uri);
	break;
      }
      cur = cur->next;
  }
  xmlFreeDoc(doc);
  #endif
 int u8loop=0;
 char Temp_Adr[150]={0};
 bool directory_hata=false;
 
 char *project_count;//main disinda ki prj sayisi
 char *main_sha;//main dosya sha
 char *main_file_count;//main dosya sayisi
 char *main_filename;//main dosya adi
  project_count=read_attribute("/home/metin/Desktop/dnm1/xml/ds2.xml","TV_Info", "ProjectCount");
  //printf("project_count=%s \n",project_count);
  //DS_TEMP veya DS_TEMP icerisinde xml dosyasi yok ise dire hepsini indir.

  //1. DS ana dizin tümüyle yok ise
  if (!check_usb_dir("/home/metin/Desktop/dnm1/xml/Test/DS",0,0))
  	{
  	 //printf(" DS ana dizin yok Hepsini yükle.....\n");
	 directory_hata=true;
	 //Dizinleri olustur
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Main");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Main/MainAudio");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Main/MainVideo");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Main/MainPicture");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Main/MainText");

	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainVideo");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainPicture");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainText");
     
	 for(u8loop=1;u8loop<=atoi(project_count);u8loop++)
	 	{
	 	   
	 	  	//create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN");
			sprintf(Temp_Adr,"%s%d","/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Prj",u8loop);
	 	  	create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Prj",u8loop,"Prj",u8loop,"Audio");
	 	  	create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Prj",u8loop,"Prj",u8loop,"Video");
	 	  	create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Prj",u8loop,"Prj",u8loop,"Picture");
	 		create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_MAIN/Prj",u8loop,"Prj",u8loop,"Text");
	 		create_directory(Temp_Adr);

	 		//create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP");
	 		sprintf(Temp_Adr,"%s%d","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop);
	 	  	create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop,"Prj",u8loop,"Audio");
	 	  	create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop,"Prj",u8loop,"Video");
	 	  	create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop,"Prj",u8loop,"Picture");
	 		create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop,"Prj",u8loop,"Text");
	 		create_directory(Temp_Adr);
	 	}
  	}
  
  //2. DS_TEMP dizin tümüyle yok ise
   if(!check_usb_dir("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP",0,0))
  	{
  	 //printf(" DS_TEMP dizin yok Hepsini yükle.....\n");
      directory_hata=true;
	 //Dizinleri olustur
	
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainVideo");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainPicture");
	 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainText");

	 for(u8loop=1;u8loop<=atoi(project_count);u8loop++)
	 	{
	 		//create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP");
	 		sprintf(Temp_Adr,"%s%d","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop);
	 	  	create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop,"Prj",u8loop,"Audio");
	 	  	create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop,"Prj",u8loop,"Video");
	 	  	create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop,"Prj",u8loop,"Picture");
	 		create_directory(Temp_Adr);
			sprintf(Temp_Adr,"%s%d/%s%d%s","/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Prj",u8loop,"Prj",u8loop,"Text");
	 		create_directory(Temp_Adr);
	 	}
  	}
  
  //3. DS_TEMP dizin icinde ds.xml yok ise
   if(!check_usb_dir("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/ds2.xml",1,0))
  	{
  	  directory_hata=true;
  	 //printf(" DS_TEMP icinde ds.xml yok Hepsini yukle .....\n");
  	}

   if(directory_hata)
   	{
   	  //printf(" Ana Dizin veya xml dosya yok Hepsini Yukle ve islemden cik.....\n");
   	}

 // count_directory("/home/metin/Desktop/dnm1/xml/Test/DS/temp");
 //get_filename("/home/metin/Desktop/dnm1/xml/Test/DS/temp");
 //count_file("/home/metin/Desktop/dnm1/xml");
 //char command_return[100]={0};
 //sytem_command("ls -l | grep ^d | wc -l",command_return);
//printf("command retunr:%d.... \n",atoi(command_return));

 int file_count=0;
 char command_return[100]={0};
 
// 4. DS_TEMP Main dizin icerisinde dizin ve dosya kontrolu
  if(!check_usb_dir("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main",0,0))
	  {
	    //printf("DS/DS_TEMP/Main dizin bulunmuyor. Tum main dizin olustur ve kopyala...\n");
		create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio");
	 	create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainVideo");
	 	create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainPicture");
	 	create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainText");
	   }
  else 
  	{
  	  //printf("DS/DS_TEMP/Main dizin var...\n");
  	  if(!check_usb_dir("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio",0,0))
  	  	{
  	  	 //printf("DS/DS_TEMP/Main/MainAudio dizin bulunmuyor. Tum main dizin olustur ve kopyala...\n");
		 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio");
  	  	}
	  else
	  	{
	  	  search_copy_replace_file("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio/");
	  	   #if 0
	  	  int xml_file_loop=0;
		  int file_loop=0;
		  int filecount_infolder=0;
		  char tempcommand[50]={0};
		  char tempcommand2[150]={0};
		  bool dosyayi_indir=false;
		  bool same_dif_file_detect[25]={0};
	  	  printf("DS/DS_TEMP/Main/MainAudio dizin var.Dosya copy islemi baslat..\n");
		  sytem_command("ls -l /home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio | grep ^- | wc -l",command_return);
			filecount_infolder=atoi(command_return);
		  printf("dosya sayisi:%d.... \n",filecount_infolder);
		  main_file_count=read_attribute("/home/metin/Desktop/dnm1/xml/ds2.xml","MainAudio", "filecount");
  		  printf("main_file_count=%s \n ..................\n",main_file_count);
		  for(xml_file_loop=1;xml_file_loop<=atoi(main_file_count);xml_file_loop++)
		  	{
		  	    sprintf(tempcommand,"%s%d","sha",xml_file_loop);
		  		main_sha=read_attribute("/home/metin/Desktop/dnm1/xml/ds2.xml","MainAudio",tempcommand );
  		  		printf("xml_file_loop %d =%s \n ............\n",xml_file_loop,main_sha);
				file_loop=1;
				for (file_loop=1;file_loop<=filecount_infolder;file_loop++)
					{
					   sprintf(tempcommand2,"ls /home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio/ | sort -n | head -%d | tail -1",file_loop);
						printf("....file_loop=%d %s\n",file_loop,tempcommand2);
						memset(&command_return[0], 0, sizeof(command_return));
					   sytem_command(tempcommand2,command_return);
					   //printf("....%s\n",command_return);
					   sprintf(tempcommand2,"sha1sum /home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainAudio/%s | cut -d ' ' -f 1",command_return);
						//printf("....%s\n",tempcommand2);
					  sytem_command(tempcommand2,command_return);
					  //printf("....%s\n",command_return);
					  if(strcmp(command_return, main_sha) == 0)
					  	{
					  	  
						  printf("SHA COMPARING.....%s == %s .......BU DOSYANIN AYNISI VAR ISIM DEGISMELI....\n",command_return,main_sha);
						  dosyayi_indir=false;
						  same_dif_file_detect[file_loop-1]|=1;//benzer dosyalar bulunuyor
						  break;
					  	}
					  else
					  	{
					  	  printf("SHA COMPARING.....%s != %s .......BU DOSYA INDIRILMELI....\n",command_return,main_sha);
						  dosyayi_indir=true;
					  	}
					}

				if(dosyayi_indir)
					{
					    sprintf(tempcommand,"%s%d","file",xml_file_loop);
		  				main_filename=read_attribute("/home/metin/Desktop/dnm1/xml/ds2.xml","MainAudio",tempcommand );
					    printf(".......BU DOSYA (%s) INDIRILMELI....\n",main_filename);
						
					}
		  	}
		  for(file_loop=0;file_loop<filecount_infolder;file_loop++)
					{
					 if(same_dif_file_detect[file_loop])
					 	{
					 	   printf("....%d. dosya ayni kalsin\n",file_loop);
					 	}
					 else
					 	{
					 		printf("....%d. dosya farkli. Silinmeli.\n",file_loop);
					 	}
					}

		  printf(".....ISLEM TAMAMLANDI TEMP DIZINDEN ANA DIZINE KOPYALAMA ISLEMI GERCEKLESTIR \n ..................\n",main_file_count);
		#endif

		
		  
	  	}
	  if(!check_usb_dir("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainPicture",0,0))
  	  	{
  	  	 //printf("DS/DS_TEMP/Main/MainPicture dizin bulunmuyor. Tum main dizin olustur ve kopyala...\n");
		 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainPicture");
  	  	}
	  else
	  	{
	  	  search_copy_replace_file("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainPicture/");
	  	 //printf("DS/DS_TEMP/Main/MainPicture dizin var.Dosya copy islemi baslat..\n");
	  	}
	  if(!check_usb_dir("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainVideo",0,0))
  	  	{
  	  	 //printf("DS/DS_TEMP/Main/MainVideo dizin bulunmuyor. Tum main dizin olustur ve kopyala...\n");
		 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainVideo");
  	  	}
	  else
	  	{
	  	search_copy_replace_file("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainVideo/");
	  	//printf("DS/DS_TEMP/Main/MainVideo dizin var.Dosya copy islemi baslat..\n");
	  	}
	  if(!check_usb_dir("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainText",0,0))
  	  	{
  	  	 //printf("DS/DS_TEMP/Main/MainText dizin bulunmuyor. Tum main dizin olustur ve kopyala...\n");
		 create_directory("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainText");
  	  	}
	  else
	  	{
	  	search_copy_replace_file("/home/metin/Desktop/dnm1/xml/Test/DS/DS_TEMP/Main/MainText/");
	  	  //printf("DS/DS_TEMP/Main/MainText dizin var.Dosya copy islemi baslat..\n");
	  	}
  	}


  return (1);
}
