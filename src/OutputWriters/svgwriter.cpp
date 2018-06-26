#include "svgwriter.h"

namespace AutoTrace {

void SvgWriter::outSplines (FILE *fd, SplineListArray shape, int height)
{
    unsigned thisList;
    SplineList *list;
    Color lastColor(0, 0, 0);

    auto length = shape.length ();
    for (thisList = 0; thisList < length; thisList++)
    {
        unsigned thisSpline;
//        Spline first;

        list = &shape.elt (thisList);
        Spline first = list->Elt (0);

        if (thisList == 0 || !list->color.equal (&lastColor))
        {
            if (thisList > 0)
            {
                if (!(shape.centerline || list->open))
                    fputs ("z", fd);

                fputs ("\"/>\n", fd);
            }


        fprintf (fd, "<path style=\"%s:#%02x%02x%02x; %s:none;\" d=\"",
                 (shape.centerline || list->open) ? "stroke" : "fill",
                 list->color.getR (),
                 list->color.getG (),
                 list->color.getB (),
                 (shape.centerline || list->open) ? "fill" : "stroke");
        }

        fprintf(fd, "M%g %g",
                first.StartPointValue ().x,
                height - first.StartPointValue ().y);

        for (thisSpline = 0; thisSpline < list->length; thisSpline++)
        {
            Spline s = list->Elt (thisSpline);

            if (s.getDegree () == LINEAR)
            {
                fprintf (fd, "L%g %g",
                         s.EndPointValue ().x,
                         height - s.EndPointValue ().y);
            }
            else
            {
                fprintf (fd, "C%g %g %g %g %g %g",
                         s.Control1Value ().x,
                         height - s.Control1Value ().y,
                         s.Control2Value ().x,
                         height - s.Control2Value ().y,
                         s.EndPointValue ().x,
                         height - s.EndPointValue ().y);
            }

            lastColor = list->color;
        }
    }

    if (!(shape.centerline || list->open))
        fputs ("z", fd);

    if (shape.length () > 0)
        fputs("\"/>\n", fd);
}

int SvgWriter::func (FILE *fd,
                     std::string filename,
                     int llx,
                     int lly,
                     int urx,
                     int ury,
                     OutputOptions *opts,
                     SplineListArray shape
) {
    int width = urx - llx;
    int height = ury - lly;

    try {
        fd = fopen(filename.c_str(), "wb");
        fputs("<?xml version=\"1.0\" standalone=\"yes\"?>\n", fd);
        fprintf (fd, "<svg width=\"%d\" height=\"%d\">\n", width, height);

        outSplines (fd, shape, height);
        fputs ("</svg>\n", fd);
    } catch (...) {
        fclose (fd);
        throw "Error happened!";
    }

    fclose (fd);

    return 0;
}

}
