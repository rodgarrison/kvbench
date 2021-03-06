#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string>

struct Config {
  enum Mode {
    CONVERT_TEXT  = 0,
    CONVERT_TEXT1 = 1,
    UNDEFINED     = 99,
  };

  Config()
  : d_mode(UNDEFINED)
  , d_verbosity(0)
  , d_cstringTerminator(false)
  {
  }

  Mode            d_mode;
  std::string     d_inFilename;
  std::string     d_outFilename;
  unsigned int    d_verbosity;
  bool            d_cstringTerminator;
};

Config config;

void usageAndExit() {                                                                                                   
  printf("usage: generate [options]\n");                                                                               
  printf("\n");
  printf("       -m <mode>                mandatory: action to take\n");                                    
  printf("                                'convert-text' : convert whitespace delimited words in <inputFilename> to\n");
  printf("                                                 'bin-text' format writing into <outputFileName>\n");
  printf("                                'convert-text1': like 'convert-text' but write format as described by Cuckoo\n");
  printf("                                                 Trie at https://github.com/cuckoo-trie/cuckoo-trie-code\n");
  printf("\n");
  printf("       -i <inputFilename>       optional : required when <mode> is 'convert-text' otherwise not used\n");
  printf("\n");
  printf("       -o <outputFilename>      mandatory: output file\n");
  printf("\n");
  printf("       -t                       optional : add 0 terminator to each string on write\n");
  printf("                                           valid for 'convert-text' for uses where 0 terminator required\n");
  printf("\n");
  printf("       -v                       optional : increase verbosity of output\n");
  exit(2);
}

void parseCommandLine(int argc, char **argv) {                                                                          
  int opt;
  const char *switches = "m:i:o:tv";

  while ((opt = getopt(argc, argv, switches)) != -1) {
    switch (opt) {
      case 'm':
        {
          if (0==strcmp(optarg, "convert-text")) {
            config.d_mode = Config::CONVERT_TEXT;
          } else if (0==strcmp(optarg, "convert-text1")) {
            config.d_mode = Config::CONVERT_TEXT1;
          } else {
            usageAndExit();
          }
        }
        break;

      case 'i':
        {
          if (strlen(optarg)>0) {
            config.d_inFilename = optarg;
          } else {
            usageAndExit();
          }
        }
        break;

      case 'o':
        {
          if (strlen(optarg)>0) {
            config.d_outFilename = optarg;
          } else {
            usageAndExit();
          }
        }
        break;

      case 't':
        {
          config.d_cstringTerminator = true;
        }
        break;

      case 'v':
        {
          ++config.d_verbosity;
        }
        break;

      default:
        {
          usageAndExit();
        }
    }
  }

  if (config.d_mode==Config::UNDEFINED) {
    usageAndExit();
  }
  if (config.d_outFilename.empty()) {
    usageAndExit();
  }
  if (config.d_mode==Config::CONVERT_TEXT && config.d_inFilename.empty()) {
    usageAndExit();
  }
}

int convertTextHelper(int fid, char *data, const char *end, unsigned int& words) {
  char *ptr = data;

  while(ptr!=end) {
    // strip leading whitespaces
    while(isspace(*ptr)) {
      ++ptr;
    }

    // Find end-of-word
    char *start(ptr);
    for(; ptr<end; ++ptr) {
      if (isspace(*ptr)) {
        break;                                                                                                            
      }
    }

    // found word: write to output
    const char terminator(0);
    const unsigned int sz = ptr-start;
    unsigned int outputSize(sz);

    if (sz) {
      if (config.d_cstringTerminator) {
        ++outputSize;
      }
      if (write(fid, &outputSize, sizeof(outputSize))==-1) {
        printf("write error: %s (errno=%d)\n", strerror(errno), errno);
        return -1;
      }
      if (write(fid, start, sz)==-1) {
        printf("write error: %s (errno=%d)\n", strerror(errno), errno);
        return -1;
      }
      if (config.d_cstringTerminator) {
        if (write(fid, &terminator, sizeof(terminator))==-1) {
          printf("write error: %s (errno=%d)\n", strerror(errno), errno);
          return -1;
        }
      }
      ++words;
      if (config.d_verbosity) {
        printf("word: %09d, size: %05u, data '", words, sz);
        for (unsigned int i=0; i<sz; ++i) {
          putchar(*(start+i));
        }
        printf("'\n");
      } else if (words%10000==0) {
        printf("wrote %09u words\r", words);
      }
    }
  }

  printf("wrote %09u words\n", words);

  return 0;
}

int convertTextHelper1(int fid, char *data, const char *end, unsigned long& words, unsigned long& totalKeySize) {
  char *ptr = data;

  while(ptr!=end) {
    // strip leading whitespaces
    while(isspace(*ptr)) {
      ++ptr;
    }

    // Find end-of-word
    char *start(ptr);
    for(; ptr<end; ++ptr) {
      if (isspace(*ptr)) {
        break;                                                                                                            
      }
    }

    // found word: write to output
    const unsigned int sz = ptr-start;
    unsigned int outputSize(sz);
    totalKeySize += outputSize;

    if (sz) {
      if (write(fid, &outputSize, sizeof(outputSize))==-1) {
        printf("write error: %s (errno=%d)\n", strerror(errno), errno);
        return -1;
      }
      if (write(fid, start, sz)==-1) {
        printf("write error: %s (errno=%d)\n", strerror(errno), errno);
        return -1;
      }
      ++words;
      if (config.d_verbosity) {
        printf("word: %09lu, size: %05u, data '", words, sz);
        for (unsigned int i=0; i<sz; ++i) {
          putchar(*(start+i));
        }
        printf("'\n");
      } else if (words%10000==0) {
        printf("wrote %09lu words\r", words);
      }
    }
  }

  printf("wrote %09lu words with %09lu bytes in keys\n", words, totalKeySize);

  return 0;
}

int convertText() {
  int fin = open(config.d_inFilename.c_str(), O_RDONLY);
  if (fin == -1) {
    printf("Cannot open '%s': %s (errno=%d)\n", config.d_inFilename.c_str(), strerror(errno), errno);
    return -1;
  }

  int fout = open(config.d_outFilename.c_str(), O_CREAT|O_EXCL|O_WRONLY, 0666);
  if (fout == -1) {
    close(fin);
    printf("Cannot create '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    return -1;
  }

  int rc;
  char *ptr;
  char *data;
  struct stat fstat;
  unsigned int left(0);
  unsigned int words(0);
  const unsigned int blockSize = 4096;

  if ((rc = stat(config.d_inFilename.c_str(), &fstat)) != 0) {
    printf("stat error on '%s': %s (errno=%d)\n", config.d_inFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    return -1;
  }

  if (fstat.st_size == 0) {
    printf("error '%s': has zero 0 bytes nothing to do\n", config.d_inFilename.c_str());
    close(fin);
    close(fout);
    return -1;
  }

  // Allocate memory [lazy approach: will not read & process in blocks]
  data = static_cast<char*>(malloc(fstat.st_size));
  if (data==0) {
    printf("cannot allocate %lu bytes\n", fstat.st_size);
    close(fin);
    close(fout);
    return -1;
  }

  // Prepare to read input file in blocks
  ptr = data;
  left = fstat.st_size;
  printf("reading '%s' ...\n", config.d_inFilename.c_str());

  // Read input file in blocks
  while (left>=blockSize) {
    rc = read(fin, ptr, blockSize);
    if ((rc==0) || static_cast<unsigned int>(rc)!=blockSize) {
      printf("\nunexpected eof: '%s'\n", config.d_inFilename.c_str());
      close(fin);
      close(fout);
      free(data);
      return -1;
    } else if (rc==-1) {
      printf("\nread error: '%s': %s (errno=%d)\n", config.d_inFilename.c_str(), strerror(errno), errno);
      close(fin);
      close(fout);
      free(data);
      return -1;
    }
    ptr += blockSize;
    left -= blockSize;
    printf("%09u bytes left\r", left);
  }

  // Read remaining bytes
  rc = read(fin, ptr, left);
  if ((rc==0) || static_cast<unsigned int>(rc)!=left) {
    printf("\nunexpected eof: '%s'\n", config.d_inFilename.c_str());
    close(fin);
    close(fout);
    free(data);
    return -1;
  } else if (rc==-1) {
    printf("\nread error: '%s': %s (errno=%d)\n", config.d_inFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    free(data);
    return -1;
  }
  printf("%09d bytes left\n", 0);

  // Write number of words - we'll seek back and overwrite at end
  if (write(fout, &words, sizeof(words))==-1) {
    printf("write error on '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    return -1;
  }

  // Prepare to write
  printf("writing '%s' ...\n", config.d_outFilename.c_str());

  // Find words and convert/write them
  if (convertTextHelper(fout, data, data+fstat.st_size, words)!=0) {
    close(fin);
    close(fout);
    free(data);
    return -1;
  }

  if (lseek(fout, 0, SEEK_SET)==-1) {
    printf("lseek error on '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    free(data);
    return -1;
  }

  // Write number of words found
  if (write(fout, &words, sizeof(words))==-1) {
    printf("\nwrite error on '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    free(data);
    return -1;
  }

  close(fin);
  close(fout);
  free(data);

  return 0;
}

int convertText1() {
  int fin = open(config.d_inFilename.c_str(), O_RDONLY);
  if (fin == -1) {
    printf("Cannot open '%s': %s (errno=%d)\n", config.d_inFilename.c_str(), strerror(errno), errno);
    return -1;
  }

  int fout = open(config.d_outFilename.c_str(), O_CREAT|O_EXCL|O_WRONLY, 0666);
  if (fout == -1) {
    close(fin);
    printf("Cannot create '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    return -1;
  }

  int rc;
  char *ptr;
  char *data;
  struct stat fstat;
  unsigned int left(0);
  unsigned long words(0);
  unsigned long totalKeySize(0);
  const unsigned int blockSize = 4096;

  if ((rc = stat(config.d_inFilename.c_str(), &fstat)) != 0) {
    printf("stat error on '%s': %s (errno=%d)\n", config.d_inFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    return -1;
  }

  if (fstat.st_size == 0) {
    printf("error '%s': has zero 0 bytes nothing to do\n", config.d_inFilename.c_str());
    close(fin);
    close(fout);
    return -1;
  }

  // Allocate memory [lazy approach: will not read & process in blocks]
  data = static_cast<char*>(malloc(fstat.st_size));
  if (data==0) {
    printf("cannot allocate %lu bytes\n", fstat.st_size);
    close(fin);
    close(fout);
    return -1;
  }

  // Prepare to read input file in blocks
  ptr = data;
  left = fstat.st_size;
  printf("reading '%s' ...\n", config.d_inFilename.c_str());

  // Read input file in blocks
  while (left>=blockSize) {
    rc = read(fin, ptr, blockSize);
    if ((rc==0) || static_cast<unsigned int>(rc)!=blockSize) {
      printf("\nunexpected eof: '%s'\n", config.d_inFilename.c_str());
      close(fin);
      close(fout);
      free(data);
      return -1;
    } else if (rc==-1) {
      printf("\nread error: '%s': %s (errno=%d)\n", config.d_inFilename.c_str(), strerror(errno), errno);
      close(fin);
      close(fout);
      free(data);
      return -1;
    }
    ptr += blockSize;
    left -= blockSize;
    printf("%09u bytes left\r", left);
  }

  // Read remaining bytes
  rc = read(fin, ptr, left);
  if ((rc==0) || static_cast<unsigned int>(rc)!=left) {
    printf("\nunexpected eof: '%s'\n", config.d_inFilename.c_str());
    close(fin);
    close(fout);
    free(data);
    return -1;
  } else if (rc==-1) {
    printf("\nread error: '%s': %s (errno=%d)\n", config.d_inFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    free(data);
    return -1;
  }
  printf("%09d bytes left\n", 0);

  // Write number of words - we'll seek back and overwrite at end
  if (write(fout, &words, sizeof(words))==-1) {
    printf("write error on '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    return -1;
  }

  // Write total key size -- we'll seek back and overwrite at end
  if (write(fout, &totalKeySize, sizeof(totalKeySize))==-1) {
    printf("write error on '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    return -1;
  }

  // Prepare to write
  printf("writing '%s' ...\n", config.d_outFilename.c_str());

  // Find words and convert/write them
  if (convertTextHelper1(fout, data, data+fstat.st_size, words, totalKeySize)!=0) {
    close(fin);
    close(fout);
    free(data);
    return -1;
  }

  if (lseek(fout, 0, SEEK_SET)==-1) {
    printf("lseek error on '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    free(data);
    return -1;
  }

  // Write number of words found
  if (write(fout, &words, sizeof(words))==-1) {
    printf("\nwrite error on '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    free(data);
    return -1;
  }

  // Write total key size
  if (write(fout, &totalKeySize, sizeof(totalKeySize))==-1) {
    printf("write error on '%s': %s (errno=%d)\n", config.d_outFilename.c_str(), strerror(errno), errno);
    close(fin);
    close(fout);
    return -1;
  }

  close(fin);
  close(fout);
  free(data);

  return 0;
}


int main(int argc, char **argv) {
  parseCommandLine(argc, argv);                                                                                         
  if (config.d_mode==Config::CONVERT_TEXT) {
    return convertText();
  } else if (config.d_mode==Config::CONVERT_TEXT1) {
    return convertText1();
  }
  return 1;
}
