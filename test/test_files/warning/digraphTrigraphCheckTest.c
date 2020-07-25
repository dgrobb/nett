%:define DIGRAPH 'd'
??=define TRIGRAPH 't'

// A constant
const char global <: :> = "A string??/n";
// Another one <:  ??(  ??)
const char oneMore ??( ??) = "Another ??|";
// And one more
const char another[] = "??-  <%d>";

/*
      <% %>   ??)
*/
int blah(void) <%
    // no body here
%>

// A random function <%d>
int main(int argc, char** argv) ??<
    return 0;
??>