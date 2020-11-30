# Computer graphics project

First OpenGL programming exercice for the course Computer Graphics and Interactive Systems at the University of Ioannina.

## Main tasks
* Task 1
  * - [x] Make the cube 100 X 100 cube 
  * - [x] Make cube transparent
  * - [x] random colour
  * - [x] black background
  * - [x] ESC to exit
* Task 2
  * - [x] Spacebar to spawn object
  * - [x] Make objects move
  * - [ ] Make objects collide
  * - [x] Random objects
  * - [ ] Include lighting
* Task 3
  * - [X] Create shpere 
  * - [x] Tranlate inside main cube
  * - [x] Make sphere move
* Task 4
  * - [x] Camera on all axis
* Task 5
  * - [x] Texture on sphere
  * - [x] Toggle texture (T button)

# ΑΝΑΦΟΡΑ ΠΡΟΓΡΑΜΜΑΤΙΣΤΙΚΗΣ ΑΣΚΗΣΗΣ Ι

## Shaders
Οι shaders είναι υπεύθυνοι για την επικοινωνία μεταξύ του επεξεργαστή και της κάρτας γραφικών. Γράφοντας μικρά προγράμματα 
για τον vertex shader και τον fragment shader μπορούμε να καθορίσουμε την μορφή των αντικειμένων της σκηνής καθώς και το 
χρώμα τους.

## Έγγραφο .shader
Στην υλοποίηση μας χρησιμοποιούμε ένα αρχείο source code και για τους δύο shaders. Η απόφαση αυτή πάρθηκε, καθώς εξυπηρετούσε
 η ομαδοποίηση τους με αυτόν τον τρόπο. Η διαφοροποίηση τους επιτυγχάνεται με την επικεφαλίδα #shader και το είδος του shader 
 (vertex, fragment αντίστοιχα). Περισσότερες λεπτομέρειες ως προς τον τρόπο ανάγνωσης δίνονται στην ενότητα ShaderProgram. 

Μέσα στον vertex shader περιέχονται τα εξής.
Αρχικά οι δύο μεταβλητές που περιέχονται μέσα σε όλα τα μοντέλα αντικειμένου 

vertexPosition_modelspace: πρόκειται για ένα vector 3 μεταβλητών και περιέχει τις συντεταγμένες 
 του αντικειμένου στο σύστημα συντεταγμένων local (model) space. 

vertexUV: πρόκειται για ένα vector 2 μεταβλητών και περιέχει τις συντεταγμένες του texture που είναι 
συνδεδεμένο με το αντικείμενο. 

Το vector vertexPosition χρησιμοποιείται μέσα στον vertex shader, ενώ το vertexUV ωθείται στον fragment shader.

Ο vertex shader επίσης περιέχει τα transformation matrices για κάθε σύστημα συντεταγμένων ως uniforms. 
To model matrix ορίζεται πριν από κάθε draw call ενός αντικειμένου και μετατρέπει τα local coordinates στο παγκόσμιο σύστημα συντεταγμένων.
Έπειτα, το view matrix το οποίο ανανεώνεται μετά από κάθε αλλαγή της κάμερας, μετατρέπει τις συντεταγμένες από το παγκόσμιο σύστημα στο οπτικό σύστημα συντεταγμένων (που ακριβώς είναι το αντικείμενο ως προς την κάμερα). 

Τέλος, το projection matrix (στο οποίο χρησιμοποιούμε προοπτική όψη) εξυπηρετεί 2 σκοπούς. Ο πρώτος είναι να καθορίζει πόσο μακριά και πόσο κοντά στην κάμερα μπορούμε να δούμε και ο δεύτερος είναι τα αντικείμενα όσο απομακρύνονται από την κάμερα να μικραίνουν (αποτέλεσμα της προοπτικής όψης), το οποίο μας δίνει στην αίσθηση ότι κοιτάμε τρισδιάστατα αντικείμενα.

Εξήγηση της εξίσωσης του fragment shader (μπαίνει εδώ)

 
## Η κλάση ShaderProgram
Η κλάση αυτή είναι υπεύθυνη για την υλοποίηση ενός «διαμεσολαβητή» μεταξύ των shaders και του επεξεργαστή, που χρησιμοποιούμε κυρίως για τον ορισμό των transformation matrices.
Για την αρχικοποίηση του προγράμματος διαχείρησης των shaders, περνάμε στον constructor την διαδρομή προς τον χώρο που είναι αποθηκευμένο το αρχείο .shader. Εσωτερικά της κλάσης υπάρχει μια δομή shaderProgramSource (στο header της κλάσης), η οποία αποτελείται από δύο string το VertexShaderSource και το FragmentShaderSource όπου και αποθηκεύουμε τον πηγαίο κώδικα του κάθε shader. 
H διαδικασία για την κατασκευή ένος shader program είναι η εξής:
1.	Mέσα στην μέθοδο Parse shader γίνεται η ανάγνωση του αρχείου .shader. Όταν το πρόγραμμα διαβάζει την επικεφαλίδα #shader vertex/fragment γνωρίζει ότι ο ακόλουθος κώδικας έχει να κάνει με τον shader που ορίζει η επικεφαλίδα και σώζει τον κώδικά του στο αντίστοιχο string.
2.	Έχοντας λάβει τα sources και των δύο shaders, περνάμε μέσα στην μέθοδο create shaders που είναι υπεύθυνη για την δημιουργία του αντικειμένου shaderProgram. Για να δημιουργήσει όμως το αντικείμενο πρέπει πρώτα να γίνει η μετάφραση των δύο shaders. Αυτό αποτελεί ευθύνη της μεθόδου compileShader
3.	Η μέθοδος compileShader λαμβάνει το είδος του shader που πρόκεται να μεταφραστεί (ορισμένες από την openGL ως GL_VERTEX_SHADER και GL_FRAGMENT_SHADER). Η μέθοδος στην συνέχεια χρησιμοποιεί τις μεθόδους της openGL για την δημιουργία του shader που θέλουμε. Αν υπάρξει κάποιο σφάλμα κατά την μετάφραση η μέθοδος εμφανίζει ένα μήνυμα λάθους. Όταν ολοκληρωθεί η διαδικασία αυτή, η μέθοδος θα επιστρέψει ένα GLuint που αντιπροσωπεύει πλέον τον δημιουργημένο shader.
4.	Το βήμα 3 πραγματοποιείται 2 φορές, μια για κάθε shader. Έπειτα η ροή επιστρέφει στην create shader, η οποία δεσμεύει τους δύο shaders με το πρόγραμμά μας και καταστρέφει τους εικονικούς shaders, αποδεσμεύοντας έτσι μνήμη.
5.	Στο τέλος αυτής της διαδικασία έχουμε κρατήσει μια μεταβλητή GLuint, η οποία περιέχει το ID του προγράμματος με το οποίο έχουμε δεσμεύει τους δύο shaders.
Άλλες λειτουργίες που εκτελεί η κλάση αυτή είναι ο ορισμός των uniforms που περιέχονται μέσα στον vertex shader, 1 για κάθε είδους uniform που χρησιμοποιούμε. Εσωτερικά της υπάρχει και η μέθοδος getUniformLocation η οποία επιστρέφει ένα GLint με την τοποθεσία της μεταβλητής του shader που επιθυμούμε να χρησιμοποιήσουμε 

Για την υλοποίηση του ShaderProgram έγινε αναφορά στο εξής βίντεο:
Shader Abstraction in OpenGL από το κανάλι The Cherno


## ΚΑΜΕΡΑ
Η δημιουργία και η διαχείριση της κάμερας γίνετε μέσα από την κλάση Camera.
Για την αρχικοποίηση της απαιτούνται η αρχική της θέση (position) που εκφράζει την θέση του
ματιού καθώς και ο ορισμός του πάνω μέρους (worldUp).
Προκειμένου να μπορούμε να εναλλάσσουμε το οπτικό πεδίο της κάμερα μας ανανεώνουμε
συνεχώς την αντίστοιχη τιμή view που είναι υπεύθυνη. Αυτό συμβαίνει μέσα στην κλάση της
κάμερας με την μέθοδο calculateViewMatrix() .
Εκεί η LookAt() δέχεται ως εξής τις παραμέτρους της αρχικής θέσης κάμερας, τον ‘’στόχο’’ προς
τον οποίο θα κοιτάει και το πάνω μέρος.
Ο ‘’στόχος’’ μας παραμένει συνέχεια σταθερός και είναι το κέντρο του κύβου (50,50,50) όπως
αντίστοιχα και το πάνω μέρος(worldUp). Αυτό που εναλλάσσουμε συνεχώς είναι η θέση της
κάμερας. Η αρχική θέση της κάμερας είναι στο (50,50,250).
Κατά το zoomIn αλλάζουμε την θέση της κάμερας με την βοήθεια μιας σταθεράς που την
έχουμε ορίσει έπειτα από δοκιμές προκειμένου να μπορεί να είναι όσο πιο ομαλή η κίνηση
αυτή της κάμερας. Αντίστοιχα και για το ZoomOut. Έτσι αλλάζουμε την θέση της κάμερας ώστε
να την μεταφέρουμε πιο κοντά και μακριά αντίστοιχα.
Όσον αφορά τις περιστροφές που θα κάνει η κάμερα αυτές γίνονται με την βοήθεια του
quaternion. Σε κάθε μία από τις περιστροφές που έχουμε επιλέγουμε η κάθε περιστροφή να
γίνετε γύρω από έναν άξονα (x και y αντίστοιχα). Εκεί επιλέγουμε την θετική και αρνητική
γωνία που θα μας επιτρέψει να κάνουμε την δεξιόστροφή και αριστερόστροφη αντίστοιχα
περιστροφή. Η επιλογή της γωνίας αντίστοιχα έγινε έπειτα από δοκιμές στην περισσότερη
επιθυμητή περιστροφή για εμάς.
Κατά την περιστροφή γύρω από έναν άξονα θεωρώντας ως κέντρο τον κύβο μας η θέση της
κάμερας δεν ανήκει σε έναν κύκλο με σταθερή ακτίνα. Συνεπώς υπάρχουν σημεία που η
κάμερα μας θα περιστρέφεται όπως επιθυμούμε κανονικά γύρω από τον αντίστοιχο άξονα
αλλά θα μεγεθύνεται ή θα μικραίνει το πλάνο πατώντας μόνο το αντίστοιχο πλήκτρο
περιστροφής. Με την χρήση των πλήκτρων για ZoomIn [Ε] και ZoomOut[W] ο χρήστης είναι σε
θέση να επιλέξει κάθε φορά ανάλογα την εστίαση η απομάκρυνση αν θέλει να φτιάξει το
πλάνο του. Έτσι έχει την δυνατότητα να εναλλάξει τις γωνίες από τις οποίες θα βλέπει τον
κύβο και τα αντικείμενα του.
Όσον αφορά τα πλήκτρα που επιτρέπουν τις παραπάνω κινήσεις είναι:
W: rotate x positive
S: rotate x negative
A: rotate y positive
D: rotate y negative
E: zoom in
X: zoom out
Πηγή: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#qua
