/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Tommy Skodje */

//TODO:USE SPRINTF TO PUT OCTAL NUMBERS INTO BUFFER

//TODO: Which imports do I need?
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

#define MAGIC_NUMBER 0x7261746D

void createArchive(char *inputDirectoryName, char *archiveName);
void extractDirectory(char *tarFileName);
void directoryRecurse(DIR *inputDirectory, char *directoryName, 
                      char *parentName, FILE *tarFile);
void fileRecurse(DIR *inputDirectory, char *inputDirectoryName, 
                 char *fileName, FILE *tarFile);
void hardLinkRecurse(char *hardLinkName, FILE *tarFile);
void printArchive(char *archiveName);
void set_inode(ino_t i, const char *f);
const char * get_inode(ino_t i);



extern char const ** Map;



//TODO: I need a main method, right?
int main(int argc, char *argv[]) {
    int cFlag, xFlag, tFlag, fFlag = 0;

    //TODO: How much memory should I allocate for the files and the option for f?
    char *directoryName;
    char *argumentForF;
    

    int getoptValue = getopt(argc, argv, ":cxtf:");
    int modes = 0;

    // Read input form the command line
    while(getoptValue != -1) {

        //TODO: Test. Remove.
        printf("getoptValue: %d\n", getoptValue);

        switch(getoptValue) {
            case 'c':
                //TODO: Test. Remove.
                printf("c flag enabled.\n");

                cFlag = 1;
                modes++;
                break;

            case 'x':
                //TODO: Test. Remove.
                printf("x flag enabled.\n");

                xFlag = 1;
                modes++;
                break;

            case 't':
                //TODO: Test. Remove.
                printf("t flag enabled.\n");

                tFlag = 1;
                modes++;
                break;

            case 'f':
                //TODO: Test. Remove.
                printf("f flag enabled.\n");

                fFlag = 1;

                // Copy the argument given for f into argumentForF.
                int length = 0;

                //TODO:TEST
                length = strlen(optarg);

                argumentForF = (char *) malloc(length * sizeof(char));

                strncpy(argumentForF, optarg, length);

                //TODO: Test. Remove.
                printf("argumentForF = %s\n", argumentForF);

                break;

            // Error: No argument specified for -f
            case ':':
                //TODO: Is this the right error message?
                fprintf(stderr, "Error: No tarfile specified.\n");
                exit(-1);

            //Error: Unrecognized option
            case '?':
                //TODO: This wasn't on the homework. Should I print this?
                fprintf(stderr, "Error: Unrecognized option -%c\n", optopt);
                exit(-1);

        }

        getoptValue = getopt(argc, argv, ":cxtf:");
    }

    // Multiple modes error
    if(modes > 1) {
        fprintf(stderr, "Error: Multiple modes specified.\n");
        exit(-1);
    }

    // No modes error
    if(modes == 0) {
        fprintf(stderr, "Error: No mode specified.\n");
        exit(-1);
    }

    // No tar file error
    //TODO: Is this the correct way to do this error?
    if(fFlag == 0) {
        fprintf(stderr, "Error: No tarfile specified.\n");
        exit(-1);
    }

    
    // copying a directory
    if(cFlag == 1) {

        // No directory target specified error
        if(argv[optind] == NULL) {
            fprintf(stderr, "Error: No directory target specified.\n");
            exit(-1);
        }

        //TODO:Test. Remove.
        printf("argv[optind]: %s\n", argv[optind]);

        int directoryNameLength = strlen(argv[optind]);
        directoryName = (char *) malloc((directoryNameLength + 1) * sizeof(char)); 

        strncpy(directoryName, argv[optind], directoryNameLength);

        //TODO: Test. Remove.
        printf("directory name: %s\n", directoryName);

        createArchive(directoryName, argumentForF);

        free(directoryName);
    }

    // Extracting a directory
    if(xFlag == 1) {
        extractDirectory(argumentForF);
    }


    // Printing a directory
    if(tFlag == 1) {
        printArchive(argumentForF); 
    }



    free(argumentForF);

    //TODO: Is this correct?
    if(Map != NULL) {
        free(Map);
    }

    return 0;
}




















// Extracts the directory from the tar file.
void extractDirectory(char *tarFileName) {
    // Open the tar file
    FILE *tarFile;
    tarFile = fopen(tarFileName, "r");

    // Target does not exist error
    if(tarFile == NULL) {
        fprintf(stderr, "Specified target(\"%s\") does not exist.\n", tarFileName);
        exit(-1);
    }

    // Check the magic number
    int magic = getw(tarFile);

    if(magic != MAGIC_NUMBER) {
        fprintf(stderr, "Error: Bad magic number (%d), should be %d.\n", magic, MAGIC_NUMBER);
        exit(-1);
    }

    /* Loop through the entries and create the necessary files, 
       directories, and hard links.
    */
    //TODO:TEST
    struct stat tarStat;
    int statVal = stat(tarFileName, &tarStat);
    long tarSize = tarStat.st_size;
    long currentOffset = 0;

    //TODO:TEST. REMOVE.
    printf("size of tar = %lu\n", tarSize);



    // int nextChar = getc(tarFile);
    // int fseekVal = fseek(tarFile, -1, SEEK_CUR);
    // if(fseekVal != 0) {
    //     perror("fseek");
    //     exit(-1);
    // }

    //TODO:TEST
    while(currentOffset < tarSize) {

        // Get the entry inode value
        int freadVal;
        long *inode = (long *) malloc(sizeof(long) * 1);
        freadVal = fread(inode, 8, 1, tarFile);
        if(freadVal != 1) {
            perror("fread");
            exit(-1);
        }

        //TODO:TEST
        currentOffset += 8;

        //TODO:Test. Remove.
        printf("Read inode value: %lu\n", *inode);

        // Get the entry name length
        int *nameLength = (int *) malloc(sizeof(int) * 1);
        freadVal = fread(nameLength, 4, 1, tarFile);
        if(freadVal != 1) {
            perror("fread");
            exit(-1);
        }

        //TODO:TEST
        currentOffset += 4;

        //TODO:Test. Remove.
        printf("Read nameLength value: %d\n", *nameLength);


        // Get the entry name

        //TODO:TEST
        char *name = (char *) malloc(sizeof(char) * (*nameLength) + 1);
        freadVal = fread(name, 1, *nameLength, tarFile);
        name[*nameLength] = '\0';

        //TODO:TEST
        currentOffset += *nameLength;

        //TODO:Test. Remove.
        printf("Read name value: %s\n", name);


        // Check if the entry is a hard link
        int isHardLink = 0;
        const char *getInodeVal = get_inode(*inode);

        //TODO:TEST REMOVE
        printf("getInodeVal = %s\n", getInodeVal);

        if(getInodeVal != NULL) {
            //TODO:Test. Remove.
            printf("Found a hard link!\n");

            isHardLink = 1;
        }

        if(isHardLink == 0) {
            //TODO:Test. Remove.
            printf("Not a hard link!\n");

            //TODO:TEST
            long inodeVal = *inode;
            printf("inodeval = %lu\n", inodeVal);

            set_inode(inodeVal, name);

            //TODO:TEST. REMOVE.
            printf("Testing get_inode for the value just inputted.\n");
            const char *output = get_inode(inodeVal);
            printf("%s\n", output);
        }


        // If the entry is a hard link, create the hard link.
        if(isHardLink == 1) {

            //TODO:TEST
            long inodeVal = *inode;
            printf("inodeval = %lu\n", inodeVal);

            const char *originalFilePath = get_inode(inodeVal);

            int linkVal = link(originalFilePath, name);

            //TODO:Test. Remove.
            printf("Creating hard link from %s to %s\n", name, originalFilePath);

            if(linkVal == -1) {
                perror("link");
                exit(-1);
            }

            // Input the hard link inode number
            struct stat linkStat;
            int statVal = stat(name, &linkStat);
            if(statVal == -1) {
                perror("stat");
                exit(-1);
            }

            linkStat.st_ino = *inode;

            free(inode);
            free(nameLength);
            free(name);

            continue;
        }


        // Get the entry mode
        int *mode = (int *) malloc(sizeof(int) * 1);

        freadVal = fread(mode, 4, 1, tarFile);
        if(freadVal != 1) {
            perror("fread");
            exit(-1);
        }

        //TODO:TEST
        currentOffset += 4;

        //TODO:Test. Remove.
        printf("read mode: %o\n", *mode);


        /* fileType:
        0 = Directory. 
        1 = Regular file. 
        2 = Executable file. 
        */
        int fileType;

        char stringMode[6];
        int sprintfVal = sprintf(stringMode, "%o", *mode);
        if(sprintfVal < 0) {
            perror("sprintf");
            exit(-1);
        }

        //TODO:Test. Remove.
        printf("stringMode: %s\n", stringMode);

        // Determine the entry type and create the appropriate new entry
        FILE *newFile;
        DIR *newDir;


        // Create the directory and input its information
        if(stringMode[0] == '4') {
            //TODO:Test. Remove.
            printf("File Type: Directory\n");
            fileType = 0;

            //TODO:WHY IS THIS GIVING ME AN ERROR, BUT COMPILES WITHOUT PROBLEM?
            int mkdirVal = mkdir(name, *mode);

            if(mkdirVal == -1) {
                perror("mkdir");
                exit(-1);
            }

            // Input the inode number of the new directory
            struct stat dirStat;
            newDir = opendir(name);
            int statVal = stat(name, &dirStat);

            dirStat.st_ino = *inode;

            //TODO:Test. Remove.
            printf("new directory inode written: %lu\n", dirStat.st_ino);

            //TODO: DO I NOT NEED TO INPUT DIRECTORY MODIFICATION TIME?
            // Set the offset properly for the next file
            int fseekVal = fseek(tarFile, 8, SEEK_CUR);
            if(fseekVal != 0) {
                perror("fseek");
                exit(-1);
            }
            
            //TODO:TEST
            currentOffset += 8;

            closedir(newDir);


        // Create the regular file and input its information
        } else {
            //TODO:Test. Remove.
            printf("File Type: Regular File\n");
            fileType = 1;

            newFile = fopen(name, "w+");
            if(newFile == NULL) {
                perror("fopen");
                exit(-1);
            }

            // Input the file content
            int fseekVal = fseek(tarFile, 8, SEEK_CUR);
            if(fseekVal != 0) {
                perror("fseek");
                exit(-1);
            }

            //TODO:TEST
            currentOffset += 8;

            // Get the file size
            long fileSize[1];
            freadVal = fread(fileSize, 8, 1, tarFile);
            if(freadVal != 1) {
                perror("fread");
                exit(-1);
            }

            //TODO:TEST
            currentOffset += 8;

            //TODO:Test. Remove.
            printf("read fileSize: %lu\n", fileSize[0]);

            // Write the file content
            char contentBuffer[fileSize[0] + 1];
            freadVal = fread(contentBuffer, 1, fileSize[0], tarFile);
            if(freadVal != fileSize[0]) {
                perror("fread");
                exit(-1);
            }
            
            FILE *fileToWriteTo = fopen(name, "a");
            if(fileToWriteTo == NULL) {
                perror("fopen");
                exit(-1);
            }

            int fwriteVal = fwrite(&contentBuffer, 1, fileSize[0], fileToWriteTo);
            if(fwriteVal != fileSize[0]) {
                perror("fwrite");
                exit(-1);
            }

            //TODO:TEST. REMOVE.
            printf("wrote %d bytes of data\n", fwriteVal);

            int fcloseVal = fclose(fileToWriteTo);
            if(fcloseVal == EOF) {
                perror("fclose");
                exit(-1);
            }


            contentBuffer[fileSize[0]] = '\0';

            // Reset the offset.
            //TODO:TEST. TRYING 16 HERE. WAS 8 BEFORE.
            long negativeOffset = 16 + fileSize[0];

            //TODO:Test. Remove.
            printf("negativeOffset: %lu\n", negativeOffset);

            //TODO:TEST.
            fseekVal = fseek(tarFile, -1 * (negativeOffset), SEEK_CUR);

            //TODO:TEST
            //currentOffset -= negativeOffset;



            // Input the file inode number
            struct stat fileStat;
            int statVal = stat(name, &fileStat);
            if(statVal == -1) {
                perror("stat");
                exit(-1);
            }

            fileStat.st_ino = *inode;

            //TODO:Test. Remove.
            printf("File inode is now: %lu\n", fileStat.st_ino);

            // Input file mode
            int chmodVal = chmod(name, *mode);
            if(chmodVal == -1) {
                perror("chmod");
                exit(-1);
            }

            //TODO:Test. Remove.
            printf("File mode is now: %o\n", fileStat.st_mode);


            // Input file modification time

            // Get the current time
            struct timeval times[2];
            struct timeval accessVals;
            int gettimeVal = gettimeofday(&accessVals, NULL);
            if(gettimeVal == -1) {
                perror("gettimeofday");
                exit(-1);
            }

            times[0] = accessVals;

            //TODO:Test. Remove.
            printf("current time (from gettimeofday()): %lu\n", accessVals.tv_sec);

            // Get the modification time from the tar file
            struct timeval modValues;
            long modTimeBuffer[1];

            freadVal = fread(modTimeBuffer, 8, 1, tarFile);

            if(freadVal != 1) {
                perror("fread");
                exit(-1);
            }

            //TODO:TEST. REMOVE.
            printf("modTimeBuffer[0]: %lu\n", modTimeBuffer[0]);

            //TODO:TEST
            currentOffset += 8;

            modValues.tv_sec = modTimeBuffer[0];
            modValues.tv_usec = 0;
            times[1] = modValues;

            // Set the times for the file
            int utimesVal = utimes(name, times);
            if(utimesVal == -1) {
                perror("utimes");
                exit(-1);
            }




            //TODO:TEST
            // Increas currentOffset by file size + file content
            currentOffset += fileSize[0] + 8;

            //TODO:TEST
            fseekVal = fseek(tarFile, fileSize[0] + 8, SEEK_CUR);
            if(fseekVal != 0) {
                perror("fseek");
                exit(-1);
            }



            fclose(newFile);
        }



        // int nextChar = getc(tarFile);
        // //TODO:Test.
        // if(feof(tarFile)) {
        //     printf("endoffile reached!\n");
        // }





        // int fseekVal = fseek(tarFile, -1, SEEK_CUR);
        // if(fseekVal != 0) {
        //     perror("fseek");
        //     exit(-1);
        // }

        free(inode);
        free(nameLength);

        //TODO:TEST
        //free(name);


        free(mode);


        //TODO:TEST. REMOVE.
        // printf("nextChar at the end of the file:%dboop\n", nextChar);
        printf("currentOffset: %lu\n", currentOffset);
        printf("Done with this entry.\n\n");
    }
}














// Creates an archive of inputDirectory as a tar file
void createArchive(char *inputDirectoryName, char *archiveName) {

    //TODO: Test. Remove.
    printf("inputDirectoryName: %s\n", inputDirectoryName);

    DIR *inputDirectory;
    inputDirectory = opendir(inputDirectoryName);

    // Check if input directory is actually a file and not a directory
    FILE *errFile;
    errFile = fopen(inputDirectoryName, "r");

    // Specified target does not exist error

    /*TODO: On the server, the word "Error: " is not printed. Should I print it?
      Also, there is a typo on the assignment error message. Should I recreate the typo?*/
    if(inputDirectory == NULL && errFile == NULL) {
        fprintf(stderr, "Error: Specified target(\"%s\") does not exist.\n", 
        inputDirectoryName);
        exit(-1);
    
    // Specified target is not a directory error
    } else if(inputDirectory == NULL) {
        fprintf(stderr, "Error: Specified target(\"%s\") is not a directory.\n", 
        inputDirectoryName);
    }

    int fcloseVal = fclose(errFile);

    if(fcloseVal == EOF) {
        perror("fclose");
        exit(-1);
    }

    // Create the tar file
    FILE *tarFile;
    //TODO: Is "w+" the correct option? We want to both read and write to the file, right?
    tarFile = fopen(archiveName, "w+");

    if(tarFile == NULL) {
        perror("fopen");
        exit(-1);
    }

    // Input the magic number at the start of the tar file
    int magicNumber = MAGIC_NUMBER;
    int fwriteVal;
    fwriteVal = fwrite(&magicNumber, 4, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Write the inode of the root directory into the tar file.
    struct stat fileStat;
    int statVal;
    statVal = stat(inputDirectoryName, &fileStat);

    if(statVal == -1) {
        perror("stat");
        exit(-1);
    }

    //TODO: Test. Remove.
    printf("root inode: %lu\n", fileStat.st_ino);

    fwriteVal = fwrite(&fileStat.st_ino, 8, 1, tarFile);
    
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }
      
    // Write the root directory name length
    int fileNameLength = strlen(inputDirectoryName);

    //TODO: Test. Remove.
    printf("root name length: %d\n", fileNameLength);

    fwriteVal = fwrite(&fileNameLength, 4, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Write the root directory file name
    char *fileName = (char *) malloc(fileNameLength * sizeof(char));
    fwriteVal = fwrite(inputDirectoryName, 1, fileNameLength, tarFile);

    if(fwriteVal != fileNameLength) {
        perror("fwrite");
        exit(-1);
    }

    // Store the inode value into the map
    set_inode(fileStat.st_ino, fileName);

    free(fileName);

    // Write the root directory file mode
    //TODO: Test. Remove.
    printf("root directory file mode %o\n", fileStat.st_mode);

    fwriteVal = fwrite(&fileStat.st_mode, 4, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Write the root directory file modification time
    //TODO: Test. Remove.
    printf("root directory file modification time %lu\n", fileStat.st_mtime);

    fwriteVal = fwrite(&fileStat.st_mtime, 8, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Recurse within the root directory
    struct dirent *item;
    errno = 0;
    readdir(inputDirectory);
    readdir(inputDirectory);
    item = readdir(inputDirectory);

    if(item == NULL && errno == 0) {
        perror("readdir");
        exit(-1);
    }

    if(item != NULL) {
        char originalDirectory[strlen(inputDirectoryName) + 1];
            strncpy(originalDirectory, inputDirectoryName, strlen(inputDirectoryName));
            originalDirectory[strlen(inputDirectoryName)] = '\0';


        char *slash = "/";

        //TODO:Test. Remove.
        //printf("inputDirectoryName: %s\n", inputDirectoryName);
        printf("originalDirectory: %s\n", originalDirectory);

        char *directoryWithSlash = strncat(inputDirectoryName, slash, 1);
        char *itemName = 
        strncat(directoryWithSlash, item->d_name, strlen(item->d_name));

        //TODO: Should I use lstat here to check if the first entry is a soft link? Does readdir return soft links
        statVal = lstat(itemName, &fileStat);

        if(statVal == -1) {
            perror("lstat");
            exit(-1);
        }

        //TODO:Test. Remove.
        printf("itemName: %s\n", itemName);
        
        if(S_ISDIR(fileStat.st_mode)) {
            directoryRecurse(inputDirectory, itemName, originalDirectory, tarFile);

            //TODO:TEST REMOVE
            printf("made it back to root (from directoryRecurse).\n");

        } else if(S_ISREG(fileStat.st_mode)) {
            fileRecurse(inputDirectory, originalDirectory, itemName, tarFile);

            //TODO:TEST REMOVE
            printf("made it back to root (from fileRecurse).\n");

        //TODO: What to do if the first entry is a link?
        } else if(S_ISLNK(fileStat.st_mode)) {

            //TODO:Test. Remove.
            printf("Found a soft link.\n");

            //TODO:Test.
            char rootDir[strlen(originalDirectory)];
            strncpy(rootDir, originalDirectory, strlen(originalDirectory));
            rootDir[strlen(originalDirectory)] = '\0';

            //TODO:Test. Remove.
            printf("rootDir: %s\n", rootDir);

            // Loop through the directory until a non-soft link is found
            while(S_ISLNK(fileStat.st_mode)) {

                item = readdir(inputDirectory);
                directoryWithSlash = strncat(originalDirectory, slash, 1);

                itemName = 
                strncat(directoryWithSlash, item->d_name, strlen(item->d_name));
                statVal = lstat(itemName, &fileStat);

                if(statVal == -1) {
                    perror("lstat");
                    exit(-1);
                }

            }

            //TODO:Test. Remove.
            //printf("original directory after searching for non-soft link: %s\n", originalDirectory);

            // After a non-soft link is found, call the appropriate recursive method.
            if(S_ISDIR(fileStat.st_mode)) {
                //TODO:TEST
                directoryRecurse(inputDirectory, rootDir, rootDir, tarFile);
            } else if(S_ISREG(fileStat.st_mode)) {
                fileRecurse(inputDirectory, rootDir, itemName, tarFile);
            }
        }

        //TODO:TEST
        // fcloseVal = fclose(tarFile);
        // if(fcloseVal == EOF) {
        //     perror("closedir");
        //     exit(-1);
        // }
    }

    //TODO:TEST
    // int closedirVal = closedir(inputDirectory);
    // if(closedirVal == -1) {
    //     perror("closedir");
    //     exit(-1);
    // }

    //TODO: THIS GIVES MUNMAP FOR SOME REASON IF UNCOMMENTED???
    // fcloseVal = fclose(tarFile);
    // if(fcloseVal == EOF) {
    //     perror("closedir");
    //     exit(-1);
    // }

    //TODO:TEST REMOVE
    printf("made it all the way back here.\n");
}











/* Recursively writes the information within the given directory
   into the tar file
*/
void directoryRecurse(DIR *inputDirectory, char *directoryName, char *parentName, FILE *tarFile) {
    //TODO:Test. Remove.
    printf("Called directoryRecurse!\n");


    //TODO:Test. Remove.
    printf("directoryName: %s\n", directoryName);
    printf("parentName: %s\n", parentName);


    if(inputDirectory == NULL) {
        perror("opendir");
        exit(-1);
    }

    struct stat dirInfo;
    int statVal = lstat(directoryName, &dirInfo);
    if(statVal == -1) {
        perror("lstat");
        exit(-1);
    }

    // Input the directory inode value
    //TODO:Test. Remove.
    printf("inode value: %lu\n", dirInfo.st_ino);

    int fwriteVal = fwrite(&dirInfo.st_ino, 8, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Input the directory file name length
    int fileNameLength = strlen(directoryName);
    //TODO: Test. Remove.
    printf("file name length: %d\n", fileNameLength);

    fwriteVal = fwrite(&fileNameLength, 4, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Input the directory file name
    fwriteVal = fwrite(directoryName, 1, fileNameLength, tarFile);

    if(fwriteVal != fileNameLength) {
        perror("fwrite");
        exit(-1);
    }

    // Input the directory mode
    //TODO:Test. Remove.
    printf("directory mode: %d\n", dirInfo.st_mode);

    fwriteVal = fwrite(&dirInfo.st_mode, 4, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Input the directory modification time
    //TODO:Test. Remove.
    printf("directory mod time: %lu\n", dirInfo.st_mtime);

    fwriteVal = fwrite(&dirInfo.st_mtime, 8, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Recurse on the next entry
    struct dirent *ent;
    errno = 0;
    ent = readdir(inputDirectory);

    if(ent == NULL && errno != 0) {
        perror("readdir");
        exit(-1);
    }

    if((strcmp(ent->d_name, ".") == 0) || (strcmp(ent->d_name, "..") == 0)) {
        //TODO:Test. Remove.
        printf("skipping first value (. or ..)\n"); 
        errno = 0;
        ent = readdir(inputDirectory);
        if(ent == NULL && errno != 0) {
            perror("readdir");
            exit(-1);
        }
    }

    if((strcmp(ent->d_name, ".") == 0) || (strcmp(ent->d_name, "..") == 0)) {
        //TODO:Test. Remove.
        printf("skipping second valuee (. or ..)\n"); 
        errno = 0;
        ent = readdir(inputDirectory);
        if(ent == NULL && errno != 0) {
            perror("readdir");
            exit(-1);
        }
    }


    while(ent != NULL) {
        

        // Keep track of the parent directory name

        char originalDirectory[strlen(directoryName) + 1];
        strncpy(originalDirectory, directoryName, strlen(directoryName));
        originalDirectory[strlen(directoryName)] = '\0';

        char *slash = "/";
        char *newEntryName = strncat(directoryName, slash, 1);

        newEntryName = strncat(directoryName, ent->d_name, strlen(ent->d_name));

        //TODO:Test. Remove.
        printf("newEntryName: %s\n", newEntryName);
        printf("originalDirectory %s\n", originalDirectory);

        int statVal = lstat(newEntryName, &dirInfo);

        //TODO:TEST.
        struct stat fileInfo;
        char fileOriginalDir[strlen(parentName) + 1];
        strncpy(fileOriginalDir, parentName, strlen(parentName));
        fileOriginalDir[strlen(parentName)] = '\0';
        char *newFileEntry = strncat(parentName, slash, 1);
        newFileEntry = strncat(parentName, ent->d_name, strlen(ent->d_name));
        int fileStatVal = lstat(newFileEntry, &fileInfo);

        //TODO:Test. Remove.
        printf("newFileEntry = %s\n", newFileEntry);
        printf("fileInfo.st_mode: %o\n", fileInfo.st_mode);

        if(statVal == -1 && fileStatVal == -1) {
            perror("lstat");
            exit(-1);
        }

        if(S_ISREG(fileInfo.st_mode) || S_ISREG(dirInfo.st_mode)) {

            //TODO: Test. Remove.
            printf("recursing on a regular file\n\n");

            if(S_ISREG(fileInfo.st_mode)) {
                //TODO:TEST. REMOVE.
                printf("1\n");
                printf("passing %s as inputDirectoryName to fileRecurse.\n", fileOriginalDir);
                printf("passing: %s as fileName to fileRecurse.\n", newFileEntry);



                //TODO:TEST.
                fileRecurse(inputDirectory, fileOriginalDir, newFileEntry, tarFile);
            } else if(S_ISREG(dirInfo.st_mode)) {
                //TODO:TEST. REMOVE.
                printf("2\n");
                printf("passing %s as inputDirectoryName to fileRecurse.\n", directoryName);
                printf("passing: %s as fileName to fileRecurse.\n", newEntryName);

                fileRecurse(inputDirectory, directoryName, newEntryName, tarFile);
            }

            //TODO: Test. Remove.
            printf("done recursing on a regular file\n\n");

        } else if(S_ISDIR(dirInfo.st_mode) || S_ISDIR(fileInfo.st_mode)) {
            //TODO: Test. Remove.
            printf("original directory within else statement: %s\n", originalDirectory);
            printf("recursing on a directory\n\n");

            //TODO:TEST.
            DIR *newInputDirectory;
            if(S_ISDIR(dirInfo.st_mode)) {
                newInputDirectory = opendir(newEntryName);
                if(newInputDirectory == NULL) {
                    perror("opendir");
                    exit(-1);
                }

                directoryRecurse(newInputDirectory, newEntryName, originalDirectory, tarFile);

            } else if(S_ISDIR(fileInfo.st_mode)){
                newInputDirectory = opendir(newFileEntry);
                if(newInputDirectory == NULL) {
                    perror("opendir");
                    exit(-1);
                }

                directoryRecurse(newInputDirectory, newFileEntry, originalDirectory, tarFile);
            }

            //TODO:Test. Remove.
            printf("done recursing on a directory. Closing the directory.\n\n");

            int closedirVal = closedir(newInputDirectory);

            if(closedirVal == -1) {
                perror("closedir");
                exit(-1);
            }

            //TODO:TEST. Reset directoryName
            directoryName = originalDirectory;
            //parentName = fileOriginalDir;

            //TODO:TEST.
            directoryName[strlen(originalDirectory)] = '\0';
            parentName[strlen(fileOriginalDir)] = '\0';

            //TODO:TEST. REMOVE.
            //printf("newEntryName: %s\n", newEntryName);
            printf("directoryName: %s\n", directoryName);



        }

        //TODO: Do nothing for a soft link, right? Since we don't want to record them.

        //TODO:TEST. REMOVE.
        printf("made it here(directory)\n");
        printf("inputDirectory: %p\n", inputDirectory);

        errno = 0;
        ent = readdir(inputDirectory);

        //TODO:Test. Remove.
        printf("made it here(directory)!!!\n");

        // if(ent == NULL && errno != 0) {
        //     perror("readdir");
        //     exit(-1);
        // }
    }
}











// Writes the information for the given file or hard link into the tar file.
void fileRecurse(DIR *inputDirectory, char *inputDirectoryName, char *fileName, FILE *tarFile) {

    //TODO:Test. Remove.
    printf("Called fileRecurse!\n");
    printf("inputDirectoryName: %s\n", inputDirectoryName);
    printf("fileName: %s\n", fileName);


    //TODO: Test. Remove.
    if(inputDirectory == NULL) {
        printf("input directory is NULL!!!\n");
    }

    FILE *currentFile;
    currentFile = fopen(fileName, "r");

    if(currentFile == NULL) {
        perror("fopen");
        exit(-1);
    }

    struct stat fileInfo;

    int statVal = lstat(fileName, &fileInfo);

    if(statVal == -1) {
        perror("stat");
        exit(-1);
    }

    
    // Write the file inode number
    //TODO:Test. Remove.
    //printf("file inode: %lu\n", fileInfo.st_ino);

    int fwriteVal;
    fwriteVal = fwrite(&fileInfo.st_ino, 8, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Write the file name length
    int nameLength = strlen(fileName);

    //TODO:Test. Remove.
    //printf("file name length: %d\n", nameLength);

    fwriteVal = fwrite(&nameLength, 4, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Write the file name
    //TODO: Test. Remove.
    printf("file name: %s\n", fileName);

    fwriteVal = fwrite(fileName, 1, nameLength, tarFile);

    if(fwriteVal != nameLength) {
        perror("fwrite");
        exit(-1);
    }

    /* Put the inode and name into the map
       and check if the file is a hard link
    */
    int isHardLink = 0;

    if(get_inode(fileInfo.st_ino) != NULL) {
        //TODO: Test. Remove.
        printf("hard link found!\n");

        isHardLink = 1;
    }

    if(isHardLink == 0) {
        //TODO:Test. Remove.
        printf("not a hard link. Storing value.\n");

        set_inode(fileInfo.st_ino, fileName);

        // Write the file mode
        //TODO: Test. Remove.
        //printf("file mode: %d\n", fileInfo.st_mode);

        fwriteVal = fwrite(&fileInfo.st_mode, 4, 1, tarFile);

        if(fwriteVal != 1) {
            perror("fwrite");
            exit(-1);
        }

        // Write the file modification time
        //TODO: Test. Remove.
        //printf("file modification time: %lu\n", fileInfo.st_mtime);

        fwriteVal = fwrite(&fileInfo.st_mtime, 8, 1, tarFile);

        if(fwriteVal != 1) {
            perror("fwrite");
            exit(-1);
        }

        // Write the file size
        //TODO: Test. Remove.
        //printf("file size: %lu\n", fileInfo.st_size);

        fwriteVal = fwrite(&fileInfo.st_size, 8, 1, tarFile);

        if(fwriteVal != 1) {
            perror("fwrite");
            exit(-1);
        }

        // Write the file content
        char fileContent[fileInfo.st_size];
        int freadVal = fread(fileContent, 1, fileInfo.st_size, currentFile);

        //TODO:Test. Remove.
        //printf("freadVal: %d\n", freadVal);

        if(freadVal != fileInfo.st_size) {
            perror("fread");
            exit(-1);
        }

        fwriteVal = fwrite(fileContent, 1, fileInfo.st_size, tarFile);

        if(fwriteVal != fileInfo.st_size) {
            perror("fwrite");
            exit(-1);
        }

    }

    int fcloseVal = fclose(currentFile);
    if(fcloseVal == EOF) {
        perror("fclose");
        exit(-1);
    }


    // read the next entry and continue
    struct dirent *item;
    errno = 0;

    item = readdir(inputDirectory);

    
    //TODO:TEST
    // if(item == NULL && errno != 0) {
    //     perror("readdir");
    //     exit(-1);
    // }

    //TODO:Test. Remove.
    printf("made it to 1255\n");


    while(item != NULL) {

        //TODO:Test. Remove.
        printf("inputDirectoryName in fileRecurse: %s\n", inputDirectoryName);

        // Keep track of the parent directory name
        char originalDirectory[strlen(inputDirectoryName) + 1];
        strncpy(originalDirectory, inputDirectoryName, strlen(inputDirectoryName));
        originalDirectory[strlen(inputDirectoryName)] = '\0';


        char *slash = "/";
        strncat(inputDirectoryName, slash, 1);
        char *itemName = 
        strncat(inputDirectoryName, item->d_name, strlen(item->d_name));

        // //TODO:Test. Remove.
        printf("original directory within fileRecurse: %s\n", originalDirectory);
        printf("next entry name: %s\n", itemName);

        statVal = lstat(itemName, &fileInfo);

        if(statVal == -1) {
            perror("lstat");
            exit(-1);
        }

        if(S_ISDIR(fileInfo.st_mode)) {
            //TODO: Test. Remove.
            printf("recursing on directory.\n\n");

            DIR *newInputDirectory = opendir(itemName);
            if(newInputDirectory == NULL) {
                perror("opendir");
                exit(-1);
            }

            directoryRecurse(newInputDirectory, itemName, originalDirectory, tarFile);

            //TODO: Test. Remove.
            printf("done recursing on a directory. closing directory\n\n");

            //TODO: Test. Remove the last appended file from the directory.
            inputDirectoryName = originalDirectory;

            //TODO:TEST.
            inputDirectoryName[strlen(originalDirectory)] = '\0';

            //TODO:TEST. REMOVE.
            //printf("newEntryName: %s\n", newEntryName);
            printf("inputDirectoryName: %s\n", inputDirectoryName);


            //TODO:TEST
            int closedirVal = closedir(newInputDirectory);

            if(closedirVal == -1) {
                perror("closedir");
                exit(-1);
            }


        } else if(S_ISREG(fileInfo.st_mode)) {
            //TODO: Test. Remove.
            printf("about to recurse on a file. inputDirectoryName for next call: %s\n", originalDirectory);
            printf("itemName: %s\n", itemName);
            printf("Recursing on a file.\n\n");


            fileRecurse(inputDirectory, originalDirectory, itemName, tarFile);

            //TODO: Test. Remove.
            printf("done recursing on a regular file\n\n");

        }

        //TODO:TEST.
        printf("made it here (file)\n");

        //TODO:Test.
        errno = 0;
        item = readdir(inputDirectory);


        //TODO:TEST
        // if(item == NULL && errno != 0) {
        //     perror("readdir");
        //     exit(-1);
        // }
    }
}






















// Prints the contents of the tar file archiveName
void printArchive(char *archiveName) {
    FILE *archive;

    archive = fopen(archiveName, "r");

    
    if(archive == NULL) {
        fprintf(stderr, "Specified target(\"%s\") does not exist.\n", archiveName);
        exit(-1);
    }

    //TODO: Are we allowed to use getw here?
    // Check the magic number
    int archiveMagic = getw(archive);  

    if(archiveMagic != MAGIC_NUMBER) {
        fprintf(stderr, "Error: Bad magic number (%d), should be %d.\n", archiveMagic, MAGIC_NUMBER);
        exit(-1);
    }


    // Print the contents of the archive

    //TODO: Test.
    //int endOfFile = feof(archive);


    //TODO: Can I use fgetc() here? feof() was not working for me for some reason. feof() keeps giving 0.
    int endOfFile = fgetc(archive);
    int fseekVal = fseek(archive, -1, SEEK_CUR);
    if(fseekVal != 0) {
        perror("fseek");
        exit(-1);
    }
    while(endOfFile != EOF) {

        int offset = 0;

        /* fileType:
        0 = Directory. 
        1 = Regular file. 
        2 = Executable file. 
        */
        int fileType;


        //TODO: For fread() should I malloc here?
        long *inode = (long *) malloc(1 * sizeof(long));
        int *fileNameLength = (int *) malloc(1 * sizeof(int));
        char *fileName;
        int *fileMode = (int *) malloc(1 * sizeof(int));

        // A string version of fileMode to properly format the print.
        char fileModeString[6];

        long *modTime = (long *) malloc(1 * sizeof(long));
        long *fileSize = (long *) malloc(1 * sizeof(long));
        int freadVal;

        int inodeCounter = 0;
        int isHardLink = 0;
        

        while(offset <= 6) {

            switch(offset) {
                // Get the inode number
                case 0:
                    freadVal = fread(inode, sizeof(long), 1, archive);

                    if(freadVal != 1) {
                        //TODO: Test. Remove.
                        printf("inode fread perror called.\n");

                        perror("fread");
                        exit(-1);
                    }

                    //TODO:Test. Remove.
                    //printf("inode: %lu\n", *inode);

                    offset++;
                    break;


                // Get the file name length
                case 1:
                    freadVal = fread(fileNameLength, sizeof(int), 1, archive);

                    if(freadVal != 1) {
                        //TODO: Test. Remove.
                        printf("fileNameLength fread perror called.\n");

                        perror("fread");
                    }

                    fileName = (char *) malloc((*fileNameLength) * sizeof(char) + 1);

                    //TODO:Test. Remove.
                    //printf("fileNameLength: %d\n", *fileNameLength);

                    offset++;
                    break;

                // Get the file name and check if the entry is a hard link
                case 2:
                    freadVal = fread(fileName, sizeof(char), *fileNameLength, archive);


                    //TODO:TEST
                    fileName[*fileNameLength] = '\0';

                    //TODO: Test. Remove.
                    //printf("*fileNameLength: %d\n", *fileNameLength);


                    if(freadVal != *fileNameLength) {
                        //TODO:Test. Remove.
                        printf("fileName perror called.\n");

                        perror("fread");
                    }

                    //TODO:Test. Remove
                    //printf("fileName: %s\n", fileName);


                    // Check if the inode value is already stored
                    const char *getInodeValue = get_inode(*inode);

                    

                    
                    //TODO:Test. Remove.
                    // printf("*getInodeValue: %s\n", getInodeValue);

                    if(getInodeValue != NULL) {
                        //TODO: Test. Remove.
                        //printf("Hard link found!\n");
                        isHardLink = 1;
                    } 

                    // Store the inode value (if it isn't already stored).
                    if(isHardLink == 0) {
                        //TODO: Test. Remove.
                        //printf("Not a hard link. Storing value.\n");
                        set_inode(*inode, fileName);
                    }

                    offset++;
                    break;

                // Get the file mode if the entry is not a hard link
                case 3:

                    if(isHardLink == 0) {

                        freadVal = fread(fileMode, sizeof(int), 1, archive);

                        if(freadVal != 1) {
                            //TODO: Test. Remove.
                            printf("fileMode fread perror called.\n");

                            perror("fread");
                            exit(-1);
                        }

                        //TODO:Test. Remove.
                        //printf("fileMode: %o\n", *fileMode);


                        // Determine if the entry is a file or a directory
                        
                        int sprintfValue = sprintf(fileModeString, "%o", *fileMode);

                        if(sprintfValue < 0) {
                            perror("sprintf");
                            exit(-1);
                        }

                        //TODO: change this to S ISDIR
                        if(fileModeString[0] == '4') {
                            //TODO: Test. Remove.
                            //printf("This entry is a directory.\n");
                            fileType = 0;

                        // Check if any execute bits are on.
                        //TODO: Is this the right way to do this???
                        } else {
                            //TODO: Am I allowed to use atoi() here?
                            int firstNum = atoi(&fileModeString[2]);
                            int secondNum = atoi(&fileModeString[3]);
                            int thirdNum = atoi(&fileModeString[4]);

                            //TODO: Test. Remove.
                            //printf("firstNum: %d", firstNum);

                            if(firstNum % 2 == 1 || secondNum % 2 == 1 || thirdNum % 2 == 1) {
                                //TODO: Test. Remove.
                                //printf("This entry is an executable file.\n");
                                fileType = 2;
                            } else {
                                //TODO: Test. Remove.
                                //printf("This entry is a regular file.\n");
                                fileType = 1;
                            }
                        }

                    }

                    offset++;
                    break;
                
                // Get the file modification time if the file is not a hard link
                case 4:
                    if(isHardLink == 0) {
                        freadVal = fread(modTime, sizeof(long), 1, archive);

                        if(freadVal != 1) {
                            //TODO: Test. Remove.
                            printf("modTime fread perror called.\n");

                            perror("fread");
                            exit(-1);
                        }

                        //TODO: Test. Remove.
                        //printf("file modification time: %lu\n", *modTime);  
                    }

                    offset++;
                    break;

                // Get the file size if the entry is not a directory or a hard link.
                case 5:
                    if(isHardLink == 0 && fileType != 0) {
                        freadVal = fread(fileSize, sizeof(long), 1, archive);

                        if(freadVal != 1) {
                            //TODO: Test. Remove.
                            printf("fileSize fread perror called.\n");

                            perror("fread");
                            exit(-1);
                        }

                        //TODO: Test. Remove.
                        //printf("file size: %lu\n", *fileSize);

                    }

                    offset++;
                    break;

                // Set the offset to the next file if the entry is a regular or executable file.
                case 6:

                    //TODO: Test. Remove.
                    //printf("case 6 fileType: %d\n", fileType);

                    if(isHardLink == 0 && (fileType == 1 || fileType == 2)) {

                        //TODO: Test. Remove.
                        //printf("Moving offset %lu bytes\n", *fileSize);

                        int fseekVal = fseek(archive, *fileSize, SEEK_CUR);

                        if(fseekVal != 0) {
                            perror("fseek");
                        }
                    }

                    offset++;
                    break;


                default:
                    offset++;
                    break; 
            }

            
        }

        // Print the output for the current file.
        
        // Format the mode correctly by removing the first 2 or 3 digits.

        //TODO:Test. Remove.
        //printf("fileModeString: %s\n", fileModeString);

        const char *fileModeFormatted;
        if(isHardLink == 0) {
            if(fileType == 0) {
                //TODO: Test. Remove.
                //printf("Formatting directory\n");
                fileModeFormatted = &fileModeString[2];
            } else if(fileType == 1 || fileType == 2) {
                //TODO: Test. Remove
                //printf("Formatting file\n");
                fileModeFormatted = &fileModeString[3];
            }
        }

        //TODO: Test. Remove.
        //printf("fileModeFormatted: %s\n", fileModeFormatted);

        //TODO: Again, am I allowed to use atoi() here?
        int fileModeOutput = atoi(fileModeFormatted);

        //TODO: Test. Remove.
        //printf("fileModeOutput: %d\n", fileModeOutput);

        

        /*TODO: Should I put "--" in between the file name and inode?
         It's in the server program, but not in the assignment.
        */

        /*TODO: Am I allowed to use %d instead of %o for the mode?
          I formatted the print to remove the excess digits at the
          start of the string, so the octal representation is now
          in a decimal number representation. 
        */
        if(isHardLink == 1) {
            printf("%s/ -- inode: %lu\n", fileName, *inode);
        } else {
            if(fileType == 0) {
                printf("%s/ -- inode: %lu, mode %d, mtime: %lu\n",
                    fileName, *inode, fileModeOutput, *modTime);
            } else if(fileType == 1) {
                printf("%s -- inode: %lu, mode: %d, mtime: %lu, size: %lu\n", 
                        fileName, *inode, fileModeOutput, *modTime, *fileSize);
            } else if(fileType == 2) {
                printf("%s* -- inode: %lu, mode: %d, mtime: %lu, size: %lu\n",
                    fileName, *inode, fileModeOutput, *modTime, *fileSize);
            }
        }

        isHardLink == 0;
        offset = 0;

            free(inode);
            free(fileNameLength);
            free(fileName);
            free(fileMode);
            free(modTime);
            free(fileSize);

            //TODO:Test.
            //endOfFile = feof(archive);

            //TODO: Test
            endOfFile = fgetc(archive);
            fseek(archive, -1, SEEK_CUR);

            //TODO: Test. Remove.
            //printf("endOfFile at the end of the loop %d\n", endOfFile);

    }



    fclose(archive);
}