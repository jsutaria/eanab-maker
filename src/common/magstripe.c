#include "magstripe.h"
#include "keyboard.h"
#include "malloc.h"
#include "printf.h"
#include "strings.h"
#include "ps2_helpers.h"

void magstripe_init(unsigned int clock_gpio, unsigned int data_gpio)
{
    ps2_write(clock_gpio, data_gpio, MAGSTRIPE_RESET_COMMAND);
    keyboard_init(clock_gpio, data_gpio);
    keyboard_use_interrupts();
}

static char * magstripe_select_dest_field(magstripe_fields_t field, magstripe_track_t* track)
{
    switch (field) {
        case MAGSTRIPE_FIELD_PAN:
            return track->pan;
        case MAGSTRIPE_FIELD_NAME:
            return track->name;
        case MAGSTRIPE_FIELD_EXPIRATION_DATE:
            return track->expiration_date;
        case MAGSTRIPE_FIELD_SERVICE_CODE:
            return track->service_code;
        case MAGSTRIPE_FIELD_DISCRETIONARY_DATA:
            return track->discretionary_data;
        default:
            return NULL;
    }
}

static magstripe_track_t * magstripe_create_track(void)
{
    magstripe_track_t * track = malloc(sizeof(magstripe_track_t));
    memset(track, '\0', sizeof(magstripe_track_t)); // empty strings
    return track;
}

static magstripe_card_t * magstripe_create_card(void)
{
    magstripe_card_t *card = malloc(sizeof(magstripe_card_t));
    card->track_1 = magstripe_create_track();
    card->track_2 = magstripe_create_track();
    return card;
}


magstripe_card_t * magstripe_read_next_card(void)
{
    magstripe_card_t *card = magstripe_create_card();
    magstripe_states_t state = 0;
    magstripe_fields_t field = 0;
    int curr_field_index = 0;

    #if MAGSTRIPE_DEBUG
        printf("[magstripe] Read card: ");
    #endif

    while (1) {
        char ch = keyboard_read_next();

        #if MAGSTRIPE_DEBUG
            printf("%c", ch);
        #endif

        // Reading track da6a
        if (state & (MAGSTRIPE_STATE_TRACK_1 | MAGSTRIPE_STATE_TRACK_2)) {
            int is_track_1 = state & MAGSTRIPE_STATE_TRACK_1;
            char field_separator = is_track_1 ? '^' : '=';

            // end of track
            if (ch == '?') {
                if (is_track_1) {
                    // end of track 1 = reset, wait for a new track sentinel
                    state &= ~MAGSTRIPE_STATE_TRACK_1;
                    field = 0;
                    curr_field_index = 0;
                } else {
                    // end of track 2 = end of this read since we only support cards with 1-2 tracks
                    (void)keyboard_read_next(); // consume the newline to confirm ack
                    break;
                }

            } else {
                // advance on field separator and on fixed width fields
                int advance_to_next_field = ch == field_separator ||
                    ((field & MAGSTRIPE_FIELD_EXPIRATION_DATE) &&
                        curr_field_index == MAGSTRIPE_EXPIRATION_DATE_SIZE - 1) ||
                    ((field & MAGSTRIPE_FIELD_SERVICE_CODE) &&
                        curr_field_index == MAGSTRIPE_SERVICE_CODE_SIZE - 1);

                if (advance_to_next_field) {
                    field <<= 1;
                    curr_field_index = 0;

                    if (!is_track_1 && field == MAGSTRIPE_FIELD_NAME) {
                        // track 2 format has no name field, so skip it
                        field <<= 1;
                    }
                }

                if (ch != field_separator) {
                    char *dest = magstripe_select_dest_field(field, is_track_1 ? card->track_1 : card->track_2);
                    dest[curr_field_index++] = ch;
                }
            }

        // Sentinel: set State = Track 1
        } else if (ch == '%') {
            state |= MAGSTRIPE_STATE_TRACK_1;
            (void)keyboard_read_next(); // consume format code ("B")
            field = MAGSTRIPE_FIELD_PAN;

        // Sentinel: set State = Track 2
        } else if (ch == ';') {
            state |= MAGSTRIPE_STATE_TRACK_2;
            field = MAGSTRIPE_FIELD_PAN;
        }
    }

    #if MAGSTRIPE_DEBUG
        printf("\n");
    #endif

    return card;
}

static void magstripe_dump_track(magstripe_track_t * track)
{
    printf("PAN: %s\nName: %s\nExpiration date: %s\nService code: %s\nDiscretionary data: %s\n",
           track->pan, track->name, track->expiration_date, track->service_code, track->discretionary_data);
}

void magstripe_dump_card(magstripe_card_t * card)
{
    printf("--- begin card at %p ---\n", card);
    printf("* track 1 *\n");
    magstripe_dump_track(card->track_1);
    printf("* track 2 *\n");
    magstripe_dump_track(card->track_2);
    printf("--- end card at %p ---\n", card);
}
