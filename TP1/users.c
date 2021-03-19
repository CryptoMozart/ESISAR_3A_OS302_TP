#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>

int main (){
    struct passwd *user;  
    struct group *group;

    uid_t uid = getuid();
    gid_t gid = getgid();
    uid_t euid = geteuid();
	gid_t egid = getegid();

    user = getpwuid(uid); //on crée user comme struct passwd et on utilise la fonction getpwuid à qui on donne en argument l'uid de l'utilisateur réel pour pouvoir récupérer son nom

    group = getgrgid(gid); //on crée group comme struct group et on utilise la fonction getgrid à qui on donne en argument le gid de l'utilisateur réel pour pouvoir récupérer son nom de groupe

    printf("Reel : %s (UID = %d) \t %s (GID = %d)\n",user->pw_name,uid,group->gr_name,gid);
//on affiche le nom et l'uid de l'utilisateur réel ainsi que le nom et le gid de son groupe
   

    user = getpwuid(euid); //on modifie user pour pouvoir récupérer cette fois le nom de l'utilisateur effectif
    group = getgrgid(egid);//de même avec group pour pouvoir récupérer le nom de groupe effectif
    
    printf("Effectif : %s (UID = %d) \t %s (GID = %d)\n", user->pw_name, geteuid(), group->gr_name, getegid());

    //on affiche le nom et l'euid de l'utilisateur effectif ainsi que le nom et le egid de son groupe
  return 0;
}


