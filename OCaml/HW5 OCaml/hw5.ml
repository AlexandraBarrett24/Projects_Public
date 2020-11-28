type pattern =
  | PWildcard
    (* the wildcard pattern `_` *)
  | PVar of string
    (* the variable pattern: `x`*)
  | PNum of int
    (* the number as the pattern *)
  | PTrue
    (* `True` *)
  | PFalse
    (* `False` *)
  | PTuple of pattern list
    (* the tuple pattern.
     * the length of the tuple should be 0 (for the nullary tuple)
     * or >= 2 (for the usual tuples). *)
  | PNone
    (* `None` *)
  | PSome of pattern
    (* `Some p` *)
  | PNil
    (* `[]` *)
  | PCons of pattern * pattern
    (* `p1 :: p2` *)

type type_ = (* `type` is a keyword so we need to use a different name *)
  | TInt
    (* type `int` *)
  | TBool
    (* type `bool` *)
  | TTuple of type_ list
    (* `a1 * a2 * ... * an` *)
    (* `unit` is represented by `TTuple []`. *)
  | TOption of type_
    (* type `a option` *)
  | TList of type_
    (* type `a list` *)

type value =
  | VNum of int
    (* values of `int` *)
  | VTrue
    (* `true` *)
  | VFalse
    (* `false` *)
  | VTuple of value list
    (* `(v1, v2, ...)` *)
  | VNone
    (* `None` *)
  | VSome of value
    (* `Some v` *)
  | VNil
    (* `Nil` *)
  | VCons of value * value
    (* `v1 :: v2` *)

(* `all2 f l1 l2` = `true` if and only if
 * (1) `l1` and `l2` have the same length, and
 * (2) `f e1 e2` = `true` for every pair of corresponding elements
 *     `e1` and `e2` in `l1` and `l2`, respectively.
 *)
let rec all2 : 'a 'b . ('a -> 'b -> bool) -> 'a list -> 'b list -> bool
  = fun f -> fun l1 -> fun l2 ->
    match (l1, l2) with
    | ([], []) -> true
    | (x1 :: l1', x2 :: l2') ->
      (match f x1 x2 with
       | true -> all2 f l1' l2'
       | false -> false)
    | _ -> false

(* `check_value ty v` = `true` if and only if the value `v` is of type `ty`. *)
let rec check_value : type_ -> value -> bool
  = fun t -> fun v ->
    match (t, v) with
    | (TInt, VNum _) -> true
    | (TBool, VTrue) -> true
    | (TBool, VFalse) -> true
    | (TTuple [_], _) -> false
    | (TTuple tys, VTuple vs) -> all2 check_value tys vs
    | (TOption _, VNone) -> true
    | (TOption ty, VSome v) -> check_value ty v
    | (TList _, VNil) -> true
    | (TList ty, VCons (vhead, vtail)) ->
      all2 check_value [ty; TList ty] [vhead; vtail]
    | _ -> false

(* Exercise 1.1:
 *
 * Implement `check_pattern` of type `type_ -> pattern -> bool`
 * such that `check_pattern ty p` returns `true` if and only if
 * the pattern represented by `p` makes sense for the type `ty`.
 *)
let rec check_pattern : type_ -> pattern -> bool
  = fun t -> fun v ->
    match (t, v) with
    | (_, PWildcard) -> true
    | (_, PVar _) -> true
    | (TInt, PNum _) -> true
    | (TBool, PTrue) -> true
    | (TBool, PFalse) -> true
    | (TTuple [_], _) -> false
    | (TTuple tys, PTuple ps) -> all2 check_pattern tys ps
    | (TOption _, PNone) -> true
    | (TOption ty, PSome p) -> check_pattern ty p
    | (TList _, PNil) -> true
    | (TList ty, PCons (phead, ptail)) ->
      all2 check_pattern [ty; TList ty] [phead; ptail]
    | _ -> false

let make_option : 'b . 'b -> 'b option =
  fun one ->
  match one with
  |x -> Some x
  |_ -> None

(* Exercise 3:
 *
 * Implement `match_value` of type `pattern -> value -> (string * value) list option`
*)
(*
  let helper_match :  pattern -> value -> (string * value) =
   fun p -> fun v ->
   match (p, v) with

   | (PWildcard, _) -> ("", VTrue)
   |_-> ("", VTrue)
   (*
   | (PVar "x", VTrue) -> Some [("x", VTrue)]
   | (PVar "x", VFalse) -> Some [("x", VFalse)]
   | (PVar "x", VNum l) -> Some [("x", VNum l)]
   | (PVar "x", VTuple tup) -> Some [("x", VNum l)]
   |*** i think i need more of type PVar
   | (PTrue, VTrue) -> Some [()]
*)
*)
 let rec match_value : pattern -> value -> (string * value) list option =
  fun p -> fun v ->
  match (p, v) with
  | (PVar x, _) -> Some [(x,v)]
  | (PNum x, VNum y) ->
    (match x = y with
      |true ->  Some []
      |_ -> None )
  | (PWildcard, _) -> Some []
  | (PTrue, VTrue) -> Some []
  | (PFalse, VFalse) -> Some []
  | (PTuple [], VTuple []) -> Some []
  | (PNone, VNone) -> Some []
  | (PNil, VNil) -> Some []
  | (PTuple (x :: y), VTuple (z :: a)) -> (match match_value x z with
                                          |Some e -> (match match_value (PTuple y) (VTuple a) with
                                                      |Some r -> Some (e @ r)
                                                      |None -> Some e)
                                          |None -> None)
  | (PCons (x, y), VCons (z, a)) -> (match (match_value x z, match_value y a) with
                                          |(Some e, Some g) -> Some (e @ g)
                                          |_ -> None)

  | (PSome x, VSome y) -> match_value x y

  |_ -> None
(*
    |true ->  Some []
    |_ -> None )

  | _ -> None
*)

(* this function might inspire you to implement a helper function like `all2` *)
let rec collect : 'a 'b . ('a -> 'b option) -> 'a list -> 'b list option
  = fun f -> fun l ->
    match l with
    | [] -> Some []
    | x::l' ->
      (match f x with
       | None -> None
       | Some x ->
         match collect f l' with
         | None -> None
         | Some l -> Some (x :: l))
