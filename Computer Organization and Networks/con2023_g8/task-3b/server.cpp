#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string>

#include <sys/sendfile.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "framework.h"
#define BONUS_TASK
#define RTSP_VERSION ("RTSP/1.0")

// Handle a server-client connection. This method is called
// for each separate connection
void handleConnection(int fd, const char *remote_addr, uint16_t remote_port)
{
    read_info info;
    MessageType currentMessage = INVALID;
    AVContext *context = nullptr;
    // TODO: find a way to generate session ids
    int session_id;
    int client_port;
    // Initially, the state is set to initialization
    // Refer to the assignment description for the state logic  
    ConnectionState currentState= INITIALIZATION;
    int cseq;
    while (true)
    {
        // Setup the read operation and read
        // all lines from the socket
        printf("------------------\n");
        setup_read(&info, fd);
        read_all_lines(&info);
        printf("Read %ld lines\n", info.numlines);
        // Parse the method and the path from the first line
        char *method = info.lines[0];
        if (!method)
            goto empty_read;
        char *methodEnd = strchr(method, ' ');
        if (!methodEnd)
            goto bad_request;
        char *pathEnd = strrchr(methodEnd + 1, ' ');
        if (!pathEnd)
            goto bad_request;
        (*methodEnd) = '\0';
        (*pathEnd) = '\0';
        char *path = methodEnd + 1;
        // Extract information from the read lines
        currentMessage = stringToMessageType(method);
        printf("The current message type is %s\n", messageTypeToString(currentMessage));
        printf("Path is %s\n", path);
        // Get the CSeq value from the received message
        char *CSeq = search_for_header(&info, "CSeq:"); 
        if (!CSeq)
            goto missing_cseq;
        CSeq = strchr(CSeq, ' ');
        if (!CSeq)
            goto missing_cseq;
        cseq = atoi(CSeq);
        printf("CSeq is %d\n", cseq);
        switch (currentMessage)
        {
        case OPTIONS:
        {
            // TODO: Implement this method!
            dprintf(fd,"RTSP/1.0 200 OK\r\nCSeq: %d\r\nPublic: OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN\r\n\r\n", cseq);
            break;
        }
        case DESCRIBE:
        {
            // TODO: Implement this method!
            
            const char* name_of_file = filename_from_path(path);
            printf("filename: %s\n",name_of_file);

            if(!fileExists(name_of_file))
            {  
                dprintf(fd, "RTSP/1.0 404 Not Found\r\nCSeq: %d\r\n\r\n",cseq);
                break;
            }

            char file_buffer[1024];
            if(getSDPInfo(name_of_file, file_buffer, 1024) != 0)
            {
                dprintf(fd, "RTSP/1.0 500 Internal Server Error\r\nCSeq: %d\r\n\r\n",cseq);
                break;
            }
            //500
            // printf("info be: %s\n",file_buffer);
            size_t content_length = strlen(file_buffer);
            dprintf(fd,"RTSP/1.0 200 OK\r\nCSeq: %d\r\nContent-Type: application/sdp\r\nContent-Length: %ld\r\n\r\n%s"
            ,cseq, content_length, file_buffer);
            break;
        }
        case SETUP:
        {
            // TODO: Implement this method!
            if(currentState != INITIALIZATION)
            {
                dprintf(fd, "RTSP/1.0 455 Method Not Valid in This State\r\nCSeq: %d\r\n\r\n",cseq);
                break;
            }
            char * port_pair = search_for_header(&info, "Transport");
            
            // printf("it be %s\n",port_pair);
            session_id = rand();
            // if(session_id){}

            std::string s(port_pair);
            int pos_of_equal = s.find('=');
            std::string actual_port_pair = s.substr(pos_of_equal+1,s.length());
    
            int pos_of_dash = actual_port_pair.find('-');
            std::string media_port_string = actual_port_pair.substr(0, pos_of_dash);
       
            
            client_port = std::stoi(media_port_string);

            const char* name_of_file_setup = filename_from_path(path);
            printf("setup file %s\n",name_of_file_setup);
            if(!fileExists(name_of_file_setup))
            {
                dprintf(fd, "RTSP/1.0 404 Not Found\r\nCSeq: %d\r\n\r\n",cseq);
                break;
            }
            context = createAVContext(name_of_file_setup, client_port);
            currentState = READY; 
            dprintf(fd,"RTSP/1.0 200 OK\r\nCSeq: %d\r\n%s\r\nSession: %d \r\n\r\n", cseq, port_pair, session_id);

            
            // dprintf(fd,"RTSP/1.0 200 OK\r\nCSeq: %d\r\n%s\r\nSession: %d \r\n\r\n", cseq, port_pair, session_id);



            break;
        }
        case PLAY:
        {
            // TODO: Implement this method!
            char * session_str = search_for_header(&info, "Session:");
             std::string s(session_str);
            char const* digits = "0123456789";
            int pos = s.find_first_of(digits);
            std::string str3 = s.substr(pos);
            int given_id = std::stoi(str3);
            // printf("given %d\n",given_id);

            if(given_id != session_id)
            {
                dprintf(fd, "RTSP/1.0 454 Session Not Found\r\nCSeq: %d\r\n\r\n",cseq);
                break;
            }



            if(currentState != READY)
            {
                dprintf(fd, "RTSP/1.0 455 Method Not Valid in This State\r\nCSeq: %d\r\n\r\n",cseq);
                break;
            }

            AVPacket avpacket;
            dprintf(fd,"RTSP/1.0 200 OK\r\nCSeq: %d\r\nSession: %d\r\n\r\n", cseq, session_id);

            currentState = PLAYING;
            while(readPacketFromContext(context,&avpacket) == 0)
            {
                // printf("%lu %d\n",avpacket.duration, avpacket.size);
                int duration = av_rescale(avpacket.duration, (context->inputStream->time_base.num) * 1000000,context->inputStream->time_base.den);
                // printf("duration %d", duration);
                
                // printf("indexes %f %f\n",(double) context->inputStream->index,(double) avpacket.stream_index);
                if(context->inputStream->index == avpacket.stream_index)
                {
                    sendAndFreePacket(context, &avpacket);
                    usleep(duration);
                }
            }
            // printf("Sesh %d\n",session_id);
            currentState = INITIALIZATION;
            // deleteAVContext(context);
            deleteAVContext(context);
            context = nullptr;

            break;
        }
        case PAUSE:
        {
            // TODO: Implement this method!
            break;
        }
        case TEARDOWN:
        {
            // TODO: Implement this method!
            char * session_str = search_for_header(&info, "Session:");
            std::string s(session_str);
            char const* digits = "0123456789";
            int pos = s.find_first_of(digits);
            std::string str3 = s.substr(pos);
            int given_id = std::stoi(str3);
            // printf("given %d\n",given_id);

            if(given_id != session_id)
            {
                dprintf(fd, "RTSP/1.0 454 Session Not Found\r\nCSeq: %d\r\n\r\n",cseq);
                break;
            }
            if(currentState == INITIALIZATION)
            {
                dprintf(fd, "RTSP/1.0 455 Method Not Valid in This State\r\nCSeq: %d\r\n\r\n",cseq);
                break;
            }
            currentState  = INITIALIZATION;
            deleteAVContext(context);
            context = nullptr;
            session_id = 0;//disable session number
            dprintf(fd,"RTSP/1.0 200 OK\r\nCSeq: %d\r\n\r\n", cseq);
            break;
        }
        default:
        {
            printf("Unknown message type\n");
        }
        }
        complete_read(&info);
    }
    printf("Done receiving\n");
    goto close;
empty_read:
    printf("Client disconnected\n");
    goto close;
bad_request:
    printf("Invalid request\n");
    goto close;
missing_cseq:
    printf("Missing CSeq header\n");
    goto close;
close:
    complete_read(&info);
    close(fd);
    deleteAVContext(context);
    context = nullptr;
    return;
}

//delete av
//teardown
//indexes