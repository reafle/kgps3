#include "common.h"

Mesh * ReadOffFile(const char *filename) {
    int i;

    // Open file
    FILE *fp;
    if (!(fp = fopen(filename, "r"))) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return 0;
    }

    // Allocate mesh structure
    Mesh *mesh = new Mesh();
    if (!mesh) {
        fprintf(stderr, "Unable to allocate memory for file %s\n", filename);
        fclose(fp);
        return 0;
    }

    // Read file
    int nverts = 0;
    int nfaces = 0;
    int nedges = 0;
    int line_count = 0;
    char buffer[1024];
    while (fgets(buffer, 1023, fp)) {
        // Increment line counter
        line_count++;

        // Skip white space
        char *bufferp = buffer;
        while (isspace(*bufferp)) bufferp++;

        // Skip blank lines and comments
        if (*bufferp == '#') continue;
        if (*bufferp == '\0') continue;

        // Check section
        if (nverts == 0) {
            // Read header 
            if (!strstr(bufferp, "OFF")) {
                // Read mesh counts
                if ((sscanf(bufferp, "%d%d%d", &nverts, &nfaces, &nedges) != 3) || (nverts == 0)) {
                    fprintf(stderr, "Syntax error reading header on line %d in file %s\n", line_count, filename);
                    fclose(fp);
                    return NULL;
                }

                // Allocate memory for mesh
                mesh->verts = new Vertex [nverts];
                assert(mesh->verts);
                mesh->faces = new Face [nfaces];
                assert(mesh->faces);
            }
        } else if (mesh->nverts < nverts) {
            // Read vertex coordinates
            Vertex& vert = mesh->verts[mesh->nverts++];
            if (sscanf(bufferp, "%f%f%f", &(vert.x), &(vert.y), &(vert.z)) != 3) {
                fprintf(stderr, "Syntax error with vertex coordinates on line %d in file %s\n", line_count, filename);
                fclose(fp);
                return NULL;
            }
        } else if (mesh->nfaces < nfaces) {
            // Get next face
            Face& face = mesh->faces[mesh->nfaces++];

            // Read number of vertices in face 
            bufferp = strtok(bufferp, " \t");
            if (bufferp) face.nverts = atoi(bufferp);
            else {
                fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
                fclose(fp);
                return NULL;
            }

            // Allocate memory for face vertices
            face.verts = new Vertex *[face.nverts];
            assert(face.verts);

            // Read vertex indices for face
            for (i = 0; i < face.nverts; i++) {
                bufferp = strtok(NULL, " \t");
                if (bufferp) face.verts[i] = &(mesh->verts[atoi(bufferp)]);
                else {
                    fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
                    fclose(fp);
                    return NULL;
                }
            }

            // Compute normal for face
            face.normal[0] = face.normal[1] = face.normal[2] = 0;
            Vertex *v1 = face.verts[face.nverts - 1];
            for (i = 0; i < face.nverts; i++) {
                Vertex *v2 = face.verts[i];
                face.normal[0] += (v1->y - v2->y) * (v1->z + v2->z);
                face.normal[1] += (v1->z - v2->z) * (v1->x + v2->x);
                face.normal[2] += (v1->x - v2->x) * (v1->y + v2->y);
                v1 = v2;
            }

            // Normalize normal for face
            float squared_normal_length = 0.0;
            squared_normal_length += face.normal[0] * face.normal[0];
            squared_normal_length += face.normal[1] * face.normal[1];
            squared_normal_length += face.normal[2] * face.normal[2];
            float normal_length = sqrt(squared_normal_length);
            if (normal_length > 1.0E-6) {
                face.normal[0] /= normal_length;
                face.normal[1] /= normal_length;
                face.normal[2] /= normal_length;
            }
        } else {
            // Should never get here
            fprintf(stderr, "Found extra text starting at line %d in file %s\n", line_count, filename);
            break;
        }
    }

    // Check whether read all faces
    if (nfaces != mesh->nfaces) {
        fprintf(stderr, "Expected %d faces, but read only %d faces in file %s\n", nfaces, mesh->nfaces, filename);
    }

    // Close file
    fclose(fp);

    // Return mesh 
    return mesh;
}

GLuint LoadTexture(const char * filename) {

    GLuint texture;

    int width, height;

    unsigned char * data;

    FILE * file;

    file = fopen(filename, "rb");

    if (file == NULL) {
        fprintf(stderr, "No texture found for %s\n", filename);
        return 0;
    }
    width = 1024;
    height = 512;
    data = (unsigned char *) malloc(width * height * 3);
    //int size = fseek(file,);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    for (int i = 0; i < width * height; ++i) {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;

    }

    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);

    return texture;
}