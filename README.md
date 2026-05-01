# MP3 TAG READER & EDITOR
Command line tool written in C to view and edit ID3v2 metadata tags in MP3 files - such as 
song title, artist name, album, year, genre, and composer without altering audio data.

---

## ID3v2 Tag Structure
| Section | Description |
| --- | --- |
| ID3 | Signature bytes, version, flags and syncsafe size |
| Frames | Indivisual metadata fields |
| Audio data | Raw MP3 audio stream |

---

## Frame ID
| Frame ID | Metadata field |
| --- | --- |
| TIT2 | Song Title |
| TPE1 | Artist name |
| TALB | Album name |
| TYER | Year |
| TCON | Genre |
| TCOM | Composer |

---

## Key features
- View Tags: Displays all available metadata fields from MP3 file
- Edit tags: Edit the specified metadata from MP3 file
- Syncsafe Integer decoding: Correctly decodes the ID3v2 syncsafe tag size
- Multi encoding support : Handles both single and two byte text encoding

## File structure
- main.c
- view.c
- edit.c
- mp3.h


