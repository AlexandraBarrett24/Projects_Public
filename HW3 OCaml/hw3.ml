(*
 * List other people in the group:
 * Teammate 1 (Ian Colson colo031):
 * Teammate 2 (Issac Glover glove080):
 * Teammate 3 (Yunong xia00045):
 *)

(* The following is your current hw1/hw1.ml *)

(* Exercise 5.1 *)
let compose_option : (int -> int option) -> (int -> int option) -> (int -> int option)
  =
  fun (f : int -> int option) ->
  fun (g : int -> int option) ->
  fun ( x : int)->
  let t : int option = (g x) in
  match t with
  |None -> None
  |Some p -> (f p)






(* Exercise 5.2 *)
let join_option : int option option -> int option
  = fun (x : int option option) ->
  match x with
  |Some i -> i
  |_ -> None


(* The following is your current hw2/hw2.ml *)

(*
 * Exercise 3.1 [1B 20pts]
 * Define a function `drop_last` of type `int list -> int list` that keeps everything except for the last one (if any).
 *)

let rec drop_last : int list -> int list =
  fun l ->
	match l with
	|[]-> []
	|[h]-> []
	|(h :: t) -> 	h :: (drop_last t)


(*
 * Exercise 3.2 [1B 20pts]
 * Define a function `add_index` of type `int list -> int list` which adds indexes to elements in the list.
turns [4; 5] into [(0,4); (1,5)]
*)
let rec helper : int list -> int -> (int * int) list =
  fun (list : int list) ->
  fun (count : int) ->
  match list with
  |[] -> []
  |(h :: t) -> (count, h) :: helper t (count + 1)

 let rec add_index : int list -> (int * int) list=
  fun f -> helper f 0

(*



 * Exercise 3.3 [1B 20pts]
 * Define a function `generate` of type `(int -> int option) -> int -> int list`
 * which repeatedly applies the function until we see `None`.
 *)

let rec generate : (int -> int option) -> int -> int list =
  fun (begin1 : int -> int option) ->
  fun (value : int) ->
  match begin1 value with
  |None -> [value]
  |Some x -> value :: (generate begin1 x)





(* Exercise 3.4 [1A 10pts + bonus 1B 3pts]
 * Define a function `duplicate_inner` of type `int list -> int list`
 * that duplicates only the "inner" elements of a list.
 *)
let rec dupl_help : int list -> int list =
  fun list : int list ->
  match list with
  |[] -> []
  |[h] -> [h]
  |(h :: t) -> (h :: h :: (dupl_help t))



 let duplicate_inner : int list -> int list =
   fun listt : int list ->
   match listt with
   |[] -> []
   |[h] -> [h]
   |(h :: t) -> (h :: (dupl_help t))


(*  Exercise 3.5 [1A 10pts + bonus 1B 3pts]
 * Define a function `delete_every` of type `int -> int list -> int list`
 * that removes every `n`th element from the list.
 *)
 let rec delete_help : int -> int -> int list -> int list =
   fun (remove : int) ->
   fun (iteration : int) ->
   fun (list1 : int list) ->
   match iteration with
   |1 -> (match list1 with
         |[] -> []
         |(h :: t) -> delete_help remove remove t)
   |_ -> match list1 with
         |[] -> []
         |(h :: t) -> (h :: (delete_help remove (iteration - 1 : int) t))


let delete_every : int -> int list -> int list =
  fun (remove : int) ->
  fun (listt : int list) ->
  match listt with
  |[] -> []
  |_ -> delete_help remove remove listt



(* Exercise 3.6 [1A 10pts + bonus 1B 3pts]
 * Define a function `stern_brocot` of type `int * int -> bool list`
 * that locates the input fraction a/b (represented by `(a,b)`) in the Stern-Brocot tree.
1) Start with low = 0/1 and high = 1/0
2) Calculate the mediant (a+c)/(b+d)
3) if target number  matches mediant, search ends
4) if target number is smaller, output false and set new high to mediant  and go to step 2
5) if target number is greater, output true, set low to new mediant  and go to step 2
 *)

let denom : int -> int -> int -> int -> int =
  fun (a : int) -> fun (b : int) -> fun (c : int) -> fun (d : int) ->
  match a*d = c*b with
  |true -> 1
  |_ -> (match a*d > c*b with
        |true -> 2
        |_ -> 3 )


let rec help_plz : int * int -> int * int -> int * int -> bool list =
  fun (target : int * int) ->
  fun (low : int * int) ->
  fun (high : int * int) ->

  let a,b = high in
  let c,d = low in
  let targetNum, targetDen = target in
  let mediantNum = a + c in
  let mediantDen = b + d in
  let mediant = (mediantNum, mediantDen) in

  match (denom mediantNum mediantDen targetNum targetDen) with
  |1 -> []
  |3 -> true :: (help_plz target mediant high)
  |_ -> false :: (help_plz target low mediant)


let stern_brocot : int * int -> bool list =
  fun (target : int * int) ->
  let high = (1,0) in
  let low = (0,1) in
  help_plz target low high
