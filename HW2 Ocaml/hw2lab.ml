(* Exercise 1.1 *)

let head : int list -> int option =
	fun l ->
	match l with
	|[]-> None
	|(x :: g) -> Some x

	;;


(* Exercise 1.2 *)

let rec last : int list -> int option =
  fun l ->
	match l with
	|[]-> None
	|(h :: []) -> Some h
	|(x :: g) -> last g

	;;


(* Exercise 1.3 *)


(*add something to a list*)
let rec snoc : int list -> int -> int list =
	fun (x : int list) (y : int) ->
 	match x with
  	| [] -> y :: []
  	| (h :: l) -> h :: (snoc l y)

;;

(*traverse the orig*)
let rec reverse : int list -> int list =
  fun l ->
  match l with
  | [] -> []
  | (x :: l) -> snoc (reverse l) x
;;





(* Exercise 1.4 *)

(*reverse the first argument and concatenate that with the second argument*)
let rec rev_append : int list -> int list -> int list =
 	fun (flip : int list) (already : int list) ->
 	match flip with
 	| [] -> already
 	| (x :: flip) -> rev_append flip (x :: already)
;;
let reverse2 : int list -> int list =
	fun l -> rev_append l []
;;








(* Exercise 1.5 *)
(* `kadane` of type `int list -> int` *)

(*
Hint: one possible solution will have the following form:
let rec kadane_helper : int list -> int * int =
fun l ->
match l with
|[] -> (0, 0)
| (x :: l ) ->
	match kadane_helper l with
	|best_anywhere, best_from_head) ->
	let best_from_head' = max 0 (x + best_from_head) in
	let best_anywhere' = max best_anywhere best_from_head' in
	(best_anywhere' , best_from_head')

let kadane : int list -> int =
  fun l ->
  match kadane_helper l with
  | (answer, _) -> answer
*)
