(*Ian Colson id:colso031 *)

module type COUNTER =
sig
  (* Read the counter. returns the varible stored intide c*)
  val read : unit -> int
(*!c read it*)
(*c:= do something else*)
  (* Increment the counter. incements ref var by one*)
  val increment : unit -> unit

  (* Reset the counter. brings it back to zero*)
  val reset : unit -> unit
end

(* Exercise 1: finish the module Counter *)

module Counter : COUNTER =
struct
  (* This `c` is hidden behind the sealing by `COUNTER`.
   * No one can tamper this internal state unless you expose it
   * to the outside world. *)
  let c : int ref = ref 0

  let read = fun () ->
  !c
  let increment = fun () ->
  c:= !c +1
  let reset = fun () ->
  c:= 0
end

module type COUNTER_TESTER =
sig
  (* An `interaction` denotes the messages sent to and received from a module
   * in one round. *)
  type interaction =
    | Read of int (* `Read n`: run `read ()` and the output is `n`. *)
    | Increment (* `Increment`: run `increment ()`. *)
    | Reset (* `Reset`: run `reset ()`. *)

  (* The exception to be raised when the behavior is unexpected. *)
  exception Unmatched

  (* Simulate one interaction. *)
  val step : interaction -> unit

  (* Simulate a list of interaction. *)
  val run : interaction list -> unit
end
(*functor-- this is a module that takes a module*)
module CounterTester : COUNTER -> COUNTER_TESTER =
  functor (C : COUNTER) -> (*we  bound c to the counter from above here*)
  struct
    type interaction = Read of int | Increment | Reset

    exception Unmatched

    (* Simulate one interaction. *)
    let step : interaction -> unit
      = fun a ->
        match a with
        | Read n -> (match C.read () = n with true -> () | false -> raise Unmatched)
        | Increment -> C.increment ()
        | Reset -> C.reset ()

    (* Simulate a list of interactions. *)
    let run : interaction list -> unit
      = fun l -> List.iter step l
  end

(* Exercise 2: write at least five interesting test cases. *)

module CT = CounterTester (Counter)
let counter_tests =
  [
    [CT.Reset; CT.Increment; CT.Read 1];
    [CT.Reset; CT.Increment; CT.Increment; CT.Read 2];
    [CT.Reset; CT.Read 0];
    [CT.Reset; CT.Reset; CT.Read 0; CT.Increment];
    [CT.Read 1];
    [CT.Reset; CT.Increment; CT.Increment; CT.Increment; CT.Read 3];
    [CT.Reset; CT.Increment; CT.Reset; CT.Read 0];
  ]

(* Run `./lab-step-off` to see the test cases in action. *)

(** TAKE-HOME PART **)

(* Exercise 3 *)

module type COUNTER_PARAMS =
sig
  (* The initial value of the tally counter. `reset ()`
   * should also turn the counter back to this value. *)
  val start : int

  (* The step value of the counter. This is the value
   * added to the counter at each `increment` call. *)
  val step : int
end

(* Uncomment and finish ParametrizedCounter.*)

module ParametrizedCounter : COUNTER_PARAMS -> COUNTER =
  functor (P : COUNTER_PARAMS) ->
  struct
  let c : int ref = ref P.start

  let read = fun () ->
  !c
  let increment = fun () ->
  c:= !c + P.step
  let reset = fun () ->
  c:= P.start
  end


module type STRING_CACHE =
sig
  (* `add (k, v)` binds the string `k` to the value `v`
   * in the storage. If `k` was already bound to some value,
   * the new binding replaces the old binding of `k`. *)

  (* `find k` returns the value where the key `k` is bound to,
   * and raises `Not_found` (the built-in exception) if `k` is
   * not bound to anything.
   *
   * To clarify, the `Not_found` exception is this one:
   * https://caml.inria.fr/pub/docs/manual-ocaml/libref/Stdlib.html#EXCEPTIONNot_found
   * Do not define your own `Not_found` exception.
   *)
  val find : string -> int

  (* `remove k` deletes the binding associated with `k`
   * from the storage. If `k` was not bound to anything,
   * `remove k` does nothing and returns `()`. *)
  val remove : string -> unit
  val add : string * int -> unit

  (* `count ()` returns the number of (retrievable) bindings
   * in the storage. *)
  val count : unit -> int
end

(* Exercise 4.1 *)
(*decided not to change function*)
(* Uncomment and finish StringCache
*)
module StringCache : STRING_CACHE =
struct
let asList : (string * int) list ref = ref []
(* `find k` returns the value where the key `k` is bound to, and raises `Not_found` (the built-in exception) if `k` is not bound to anything. *)
let find : string -> int =
fun str ->
(List.assoc str !asList)
(* `remove k` deletes the binding associated with `k` from the storage. If `k` was not bound to anything, `remove k` does nothing and returns `()`. *)
let remove : string -> unit =
fun str ->
asList:= List.remove_assoc str !asList
(* `add (k, v)` binds the string `k` to the value `v` in the storage. If `k` was already bound to some value, the new binding replaces the old binding of `k`. *)



let add : string * int -> unit =
fun kv ->
match kv with
|(k, v) -> match (find k) with
          |exception Not_found -> asList:= kv :: !asList
          |_ ->(remove k);
                asList:= kv :: !asList (*would recursivly calling add help?*)
(* `count ()` returns the number of (retrievable) bindings in the storage. *)
let count : unit -> int =
fun u ->
List.length !asList

end


module type STRING_CACHE_TESTER =
sig
  type interaction =
    (* `Add (k, v)`: run `add (k, v)`. *)
    | Add of string * int
    (* `Find (k, None)`: run `find k` and it raises `Not_found`. *)
    (* `Find (k, Some n)`: run `find k` and it returns `n`.  *)
    | Find of string * int option
    (* `Remove k`: run `remove k` *)
    | Remove of string
    (* `Count n`: run `count ()` and it returns `n`. *)
    | Count of int

  exception Unmatched
  val step : interaction -> unit
  val run : interaction list -> unit
end
(* Exercise 4.2 *)

module StringCacheTester : STRING_CACHE -> STRING_CACHE_TESTER =
  functor (C : STRING_CACHE) ->
  struct
      type interaction = Add of string * int | Find of string * int option | Remove of string | Count of int
      exception Unmatched

      let step : interaction -> unit =
      fun inter ->
        match inter with
        | Remove s -> C.remove s    (* `Remove k`: run `remove k` *)
        | Count n-> (match C.count () = n with true -> () | false -> raise Unmatched)  (* `Count n`: run `count ()` and it returns `n`. *) (*I am confused becasue the comment is telling me to return n, but the function returns unit, also am I supposed to compare the result to n?*)
        | Add (n, l) -> C.add (n, l)      (* `Add (k, v)`: run `add (k, v)`. *)
        | Find (o, Some p) -> (match C.find o  = p with true -> () |false -> raise Not_found)
        | Find (o, None) -> (raise Not_found)
        (* `Find (k, None)`: run `find k` and it raises `Not_found`. *)
        (* `Find (k, Some n)`: run `find k` and it returns `n`.  *)

(*        let find : string -> int =  fun str ->    (List.assoc str !asList)*)
        let run : interaction list -> unit =
      fun l -> List.iter step l
  end
(* Exercise 4.3 *)
module SCT = StringCacheTester (StringCache)
let string_cache_tests =
[
  [SCT.Count 0; SCT.Add ("h", 4); SCT.Count 1; SCT.Remove "h"; SCT.Count 0];
  [SCT.Add ("g", 6); SCT.Find ("g", Some 6)];
  [SCT.Count 0; SCT.Add ("u", 6); SCT.Remove "u"];
  [SCT.Count 0; SCT.Add ("u", 6); SCT.Remove "u"; SCT.Remove "u"; SCT.Count 0];
  [SCT.Count 0; SCT.Add ("h", 4); SCT.Add ("r", 4); SCT.Count 2; SCT.Remove "h"; SCT.Count 1];
  [SCT.Count 0; SCT.Add ("h", 4); SCT.Add ("h", 5); SCT.Count 1];
  [SCT.Count 0; SCT.Add ("h", 4); SCT.Add ("h", 5); SCT.Count 1; SCT.Find ("h", Some 5)];
  [SCT.Count 0; SCT.Add ("h", 4); SCT.Add ("h", 5); SCT.Count 1; SCT.Find ("h", Some 5); SCT.Remove "h"; SCT.Count 0];
  [SCT.Count 0; SCT.Add ("h", 4); SCT.Add ("h", 5); SCT.Count 1; SCT.Find ("h", Some 5); SCT.Remove "h"; SCT.Remove "h"; SCT.Count 0];
    [SCT.Count 0; SCT.Add ("h", 4); SCT.Add ("h", 5); SCT.Count 1; SCT.Find ("h", Some 5); SCT.Remove "h"; SCT.Count 0; SCT.Remove "h"; SCT.Count 0];
](* Run `git push` to see the test cases in action. *)

(*module CT = CounterTester (Counter)
let counter_tests =
  [
    [CT.Reset; CT.Increment; CT.Increment; CT.Increment; CT.Read 3];
    [CT.Reset; CT.Increment; CT.Reset; CT.Read 0];
  ]
*)
