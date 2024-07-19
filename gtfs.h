#if defined(GTFS_IMPL) && !defined(GTFS_IMPLEMENTATION)
#define GTFS_IMPLEMENTATION
#endif
#ifndef GTFS_INCLUDED
/*
    gtfs.h -- a *tiny* General Transit Feed Specification (GTFS) parser.
*/
#define GTFS_INCLUDED (1)

#include <stdbool.h>
#include <stdlib.h>

#if !defined(GTFS_API_DECL)
  #define GTFS_API_DECL extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gtfs_t gtfs_t;

typedef struct gtfs_agency_t gtfs_agency_t;
typedef struct gtfs_calendar_dates_t gtfs_calendar_dates_t;
typedef struct gtfs_calendar_t gtfs_calendar_t;
typedef struct gtfs_feed_info_t gtfs_feed_info_t;
typedef struct gtfs_routes_t gtfs_routes_t;
typedef struct gtfs_shapes_t gtfs_shapes_t;
typedef struct gtfs_stop_times_t gtfs_stop_times_t;
typedef struct gtfs_stops_t gtfs_stops_t;
typedef struct gtfs_trips_t gtfs_trips_t;

/* https://developers.google.com/transit/gtfs/reference#agencytxt */
struct gtfs_agency_t {
  // int id;
  // const char *name;
  // const char *url;
  // const char *timezone;
  // const char *lang;
  // const char *phone;
  // const char *fare_url;
  // const char *email;
  char id[10];
  char name[100];
  char url[100];
  char timezone[50];
  char lang[10];
  char phone[20];
  char fare_url[100];
  char email[100];
};

struct gtfs_calendar_dates_t {};
struct gtfs_calendar_t {};
struct gtfs_feed_info_t {};
struct gtfs_routes_t {};
struct gtfs_shapes_t {};
struct gtfs_stop_times_t {};
struct gtfs_stops_t {};
struct gtfs_trips_t {};

gtfs_t* gtfs_load_from_file(const char *path);
void gtfs_free(gtfs_t* gtfs);

struct gtfs_t {
  gtfs_agency_t agency;

  int calendar_dates_count;
  gtfs_calendar_dates_t* calendar_dates;

  int calendar_count;
  gtfs_calendar_t* calendar;

  gtfs_feed_info_t feed_info;

  int routes_count;
  gtfs_routes_t* routes;

  int shapes_count;
  gtfs_shapes_t* shapes;

  int stops_count;
  gtfs_stops_t* stops;

  int stop_times_count;
  gtfs_stop_times_t* stop_times;

  int trips_count;
  gtfs_trips_t* trips;
};

#ifdef __cplusplus
}
#endif
#endif /* GTFS_INCLUDED */

#ifdef GTFS_IMPLEMENTATION
#define GTFS_IMPL_INCLUDED (1)

#include <string.h>

#if !defined(GTFS_ASSERT)
  #include <assert.h>
  #define GTFS_ASSERT(c) assert(c)
#endif
#if !defined(GTFS_MALLOC)
  #include <stdlib.h>
  #define GTFS_MALLOC(s) malloc(s)
  #define GTFS_FREE(p)   free(p)
#endif

#ifndef _GTFS_PRIVATE
  #if defined(__GNUC__) || defined(__clang__)
    #define _GTFS_PRIVATE __attribute__((unused)) static
  #else
    #define _GTFS_PRIVATE static
  #endif
#endif

#define GTFS_INITIAL_CAPACITY (256)
#define _GTFS_BUF_SIZE_DEF (16*1024)

/* private implementation functions */

enum {
  GTFS_AGENCY_MAX_FIELDS = 6,
  GTFS_CALENDAR_DATES_MAX_FIELDS = 6,
  GTFS_CALENDAR_MAX_FIELDS = 6,
  GTFS_FEED_INFO_MAX_FIELDS = 6,
  GTFS_ROUTES_MAX_FIELDS = 6,
  GTFS_SHAPES_MAX_FIELDS = 6,
  GTFS_STOP_TIMES_MAX_FIELDS = 6,
  GTFS_STOPS_MAX_FIELDS = 6,
  GTFS_TRIPS_MAX_FIELDS = 6,
};

/* https://developers.google.com/transit/gtfs/reference#dataset_files */
typedef enum gtfs_dataset_files_t {
  GTFS_DATASET_FILE_AGENCY,
  GTFS_DATASET_FILE_CALENDAR,
  GTFS_DATASET_FILE_CALENDAR_DATES,
  GTFS_DATASET_FILE_FEED_INFO,
  GTFS_DATASET_FILE_ROUTES,
  GTFS_DATASET_FILE_SHAPES,
  GTFS_DATASET_FILE_STOPS,
  GTFS_DATASET_FILE_STOP_TIMES,
  GTFS_DATASET_FILE_TRIPS,
} gtfs_dataset_files_t;

// _GTFS_PRIVATE char* get_next_value(char **data) {
//     char *token = strtok(*data, ",");
//     *data = strtok(NULL, ",");
//     return token;
// }

const char* get_next_value(char *currentPosition) {
    char *start = currentPosition;
    char *end = currentPosition;

    // Find the end of the current field
    while (*end != ',' && *end != '\0') {
        end++;
    }

    // Determine the length of the current field
    size_t length = end - start;

    // Allocate memory for the field value (including null-terminator)
    char *field = (char *)malloc(length + 1);

    // Copy the field value
    strncpy(field, start, length);
    field[length] = '\0'; // Null-terminate the string

    // Move to the next character after the comma (if any)
    if (*end == ',') {
        end++;
    }

    // Update the current position for the next field
    currentPosition = end;

    // Return the field value
    return field;
    // // Skip leading commas
    // while (**currentPosition == ',') {
    //     (*currentPosition)++;
    // }

    // char *start = *currentPosition;
    // int hasQuotes = 0;

    // // Check if the field is enclosed in double quotes
    // if (*start == '"') {
    //     hasQuotes = 1;
    //     start++;
    // }

    // while (*start != '\0') {
    //     if (hasQuotes && *start == '"') {
    //         // Handle quoted field
    //         start++;
    //         if (*start == '"') {
    //             // Two consecutive quotes represent a single quote
    //             start++;
    //         } else {
    //             // End of quoted field
    //             break;
    //         }
    //     } else if (!hasQuotes && *start == ',') {
    //         // End of unquoted field
    //         break;
    //     } else {
    //         start++;
    //     }
    // }

    // size_t length = start - *currentPosition;
    // if (length < size) {
    //     strncpy(dest, *currentPosition, length);
    //     dest[length] = '\0'; // Null-terminate the string
    // } else {
    //     strncpy(dest, *currentPosition, size - 1);
    //     dest[size - 1] = '\0'; // Null-terminate the string
    // }
    // *currentPosition = start;

    // return dest;
}

_GTFS_PRIVATE void _gtfs__parse_agency(gtfs_t* gtfs, char* line, size_t size) {
  // printf("%s\n", __FUNCTION__);
  // char *token = strtok(data, ",");
  // while (token != NULL) {
  //   printf("Field: %s\n", token);
  //   token = strtok(NULL, ",");
  // }
  // return;
  // printf("%s\n", __FUNCTION__);

  char *data = line;


  printf("id: %s\n", get_next_value(data));//, gtfs->agency.id, sizeof(gtfs->agency.id)));
  printf("*name: %s\n", get_next_value(data));//, gtfs->agency.name, sizeof(gtfs->agency.name)));
  printf("*url: %s\n", get_next_value(data));//, gtfs->agency.url, sizeof(gtfs->agency.url)));
  printf("*timezone: %s\n", get_next_value(data));//, gtfs->agency.timezone, sizeof(gtfs->agency.timezone)));
  printf("*lang: %s\n", get_next_value(data));//, gtfs->agency.lang, sizeof(gtfs->agency.lang)));
  printf("*phone: %s\n", get_next_value(data));//, gtfs->agency.phone, sizeof(gtfs->agency.phone)));
  printf("*fare_url: %s\n", get_next_value(data));//, gtfs->agency.fare_url, sizeof(gtfs->agency.fare_url)));
  printf("*email: %s\n", get_next_value(data));//, gtfs->agency.email, sizeof(gtfs->agency.email)));

  return;
}

_GTFS_PRIVATE void _gtfs__parse_calendar_dates(gtfs_t* gtfs, void* data, size_t size) {
  return;
}

_GTFS_PRIVATE void _gtfs__parse_calendar(gtfs_t* gtfs, void* data, size_t size) {}
_GTFS_PRIVATE void _gtfs__parse_feed_info(gtfs_t* gtfs, void* data, size_t size) {}
_GTFS_PRIVATE void _gtfs__parse_routes(gtfs_t* gtfs, void* data, size_t size) {}
_GTFS_PRIVATE void _gtfs__parse_shapes(gtfs_t* gtfs, void* data, size_t size) {}
_GTFS_PRIVATE void _gtfs__parse_stops(gtfs_t* gtfs, void* data, size_t size) {}
_GTFS_PRIVATE void _gtfs__parse_stop_times(gtfs_t* gtfs, void* data, size_t size) {}
_GTFS_PRIVATE void _gtfs__parse_trips(gtfs_t* gtfs, void* data, size_t size) {}

_GTFS_PRIVATE void _gtfs__parse(gtfs_t* gtfs, int file_index, void* data, size_t size) {
  size_t offset = strcspn(data, "\n");
  data += offset + 1;
  size -= offset - 1;

  /* choose appropriate parsing function */
  void (*fn)(struct gtfs_t*, void*, size_t);
  switch (file_index) {
  case GTFS_DATASET_FILE_AGENCY: fn = &_gtfs__parse_agency; break;
  case GTFS_DATASET_FILE_CALENDAR: fn = &_gtfs__parse_calendar; break;
  case GTFS_DATASET_FILE_CALENDAR_DATES: fn = &_gtfs__parse_calendar_dates; break;
  case GTFS_DATASET_FILE_FEED_INFO: fn = &_gtfs__parse_feed_info; break;
  case GTFS_DATASET_FILE_ROUTES: fn = &_gtfs__parse_routes; break;
  case GTFS_DATASET_FILE_SHAPES: fn = &_gtfs__parse_shapes; break;
  case GTFS_DATASET_FILE_STOPS: fn = &_gtfs__parse_stops; break;
  case GTFS_DATASET_FILE_STOP_TIMES: fn = &_gtfs__parse_stop_times; break;
  case GTFS_DATASET_FILE_TRIPS: fn = &_gtfs__parse_trips; break;
  }

  /* parse every line of data */
  size_t length;
  while ((length = strcspn(data, "\n")) > 0) {
    fn(gtfs, data, length);
    data += length + 1;
  }
}

/* public api functions */

gtfs_t* gtfs_load_from_file(const char *path) {
 	gtfs_t* gtfs = (gtfs_t*)GTFS_MALLOC(sizeof(gtfs_t));
	memset(gtfs, 0, sizeof(*gtfs));

  mz_zip_archive zip_archive;
  memset(&zip_archive, 0, sizeof(zip_archive));

  if (mz_zip_reader_init_file(&zip_archive, path, 0) == MZ_FALSE) {
    printf("Failed to initialize zip reader for %s\n", path);
  }

  printf("Files in %s:\n", path);

  void * buffer = NULL;
  size_t uncompressed_size;
  mz_zip_archive_file_stat file_stat;
  for (int i = 0; i < mz_zip_reader_get_num_files(&zip_archive); ++i) {
    if (mz_zip_reader_file_stat(&zip_archive, i, &file_stat) == MZ_TRUE) {
      printf("%s\n", file_stat.m_filename);

      uncompressed_size = file_stat.m_uncomp_size;
      buffer = GTFS_MALLOC(uncompressed_size);
      if (mz_zip_reader_extract_to_mem(&zip_archive, i, buffer, uncompressed_size, 0) == MZ_TRUE) {
        _gtfs__parse(gtfs, i, buffer, uncompressed_size);
      }
      GTFS_FREE(buffer);
    }
  }

  mz_zip_reader_end(&zip_archive);
  return gtfs;
}

void gtfs_free(gtfs_t* gtfs) {
  int i = 0;
  for (i = 0; i < gtfs->stops_count; ++i) {
    gtfs_stops_t* stop = gtfs->stops + i;
  }

  GTFS_FREE(gtfs->calendar_dates);
  GTFS_FREE(gtfs->calendar);
  GTFS_FREE(gtfs->routes);
  GTFS_FREE(gtfs->shapes);
  GTFS_FREE(gtfs->stops);
  GTFS_FREE(gtfs->stop_times);
  GTFS_FREE(gtfs->trips);
  GTFS_FREE(gtfs);
}

#endif /* GTFS_IMPLEMENTATION */